#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of states
#define MAX_STATES 100
#define MAX_TAPE_LENGTH 100
#define ASCII_RANGE 128

// Define the tape cell structure - this tape represents our linked data structure of cells
struct Cell {
    char value;
    struct Cell* next;
    struct Cell* prev;
};

// Instruction structure
struct Instruction {
    char writeSymbol;
    char moveDirection;
    int nextState;
};

struct TuringMachine {
    struct Cell* tape;
    int startState;
    int endState;
    struct Instruction transitions[MAX_STATES][ASCII_RANGE];
};

// Function to create a new tape cell with the given value as a parameter
struct Cell* createCell(char value) {
    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
    newCell->value = value;
    newCell->prev = NULL;
    newCell->next = NULL;
    return newCell;
}

// Function to move the tape head one cell to the left
struct Cell* moveLeft(struct Cell* tapeHead) {
    if (tapeHead == NULL) {
        return NULL;
    } else if (tapeHead->prev == NULL) {
        struct Cell* newCell = createCell('B'); // Extend the tape with a blank cell
        tapeHead->prev = newCell;
        newCell->next = tapeHead;
    }
    return tapeHead->prev;
}

// Function to move the tape head one cell to the right
struct Cell* moveRight(struct Cell* tapeHead) {
    if (tapeHead == NULL) {
        return NULL;
    } else if (tapeHead->next == NULL) {
        struct Cell* newCell = createCell('B'); // Extend the tape with a blank cell
        tapeHead->next = newCell;
        newCell->prev = tapeHead;
    }
    return tapeHead->next;
}

// Function to print the content of the tape
void printTape(struct Cell* tapeHead) {
    struct Cell* current = tapeHead;
    while (current != NULL) {
        printf("%c", current->value);
        current = current->next;
    }
    printf("\n");
}

// fill in values in our instruction table
void fillTuringMachine (struct TuringMachine* tm, const char* fileName) {
    FILE* file;

    file = fopen(fileName, "r");

    if (!file) {
        printf("Could not open file. Exiting...\n");
        exit(1);
    }

    char initialTape[MAX_TAPE_LENGTH];
    fgets(initialTape, sizeof(initialTape), file);
    initialTape[strlen(initialTape) - 1] = '\0'; // Remove newline character

    // Initialize the tape with the leftmost cell containing 'A'
    struct Cell* tapeStart = createCell('A');
    struct Cell* head = tapeStart;

    // Read and parse the input file
    int numStates, startState, endState;
    fscanf(file, "%d", &numStates);
    fscanf(file, "%d", &startState);
    fscanf(file, "%d", &endState);

    // declare transition struct
    struct Instruction transitionTable[MAX_STATES][ASCII_RANGE];

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int fromState, toState;
        char readVal, writeVal, moveDirection;

        if (sscanf(line, "(%d,%c)->(%c,%c,%d)", &fromState, &readVal, &writeVal, &moveDirection, &toState) != 5 || (fromState == 0 && toState == 0 && readVal == '\0' && writeVal == '\0' && moveDirection == '\0')) {
            break;
        }

        // Check if all values are either zero or NULL character
        if (fromState == 0 && toState == 0 && readVal == '\0' && writeVal == '\0' && moveDirection == '\0') {
            continue; // Skip this line and continue with the next iteration
        }

        transitionTable[fromState][(int)readVal].writeSymbol = writeVal;
        transitionTable[fromState][(int)readVal].moveDirection = moveDirection;
        transitionTable[fromState][(int)readVal].nextState = toState;

    }

    int tapeIndex = 0;

    // Allocate cells on-the-fly for the input tape
    while (initialTape[tapeIndex] != '\0') {
        struct Cell* newCell = createCell(initialTape[tapeIndex]);
        head->next = newCell;
        newCell->prev = head;
        head = newCell;
        tapeIndex++;
    }

    if (head->value =='B') {
        head->value = 'A';
    }

    printf("Initial tape contents: ");
    printTape(tapeStart);

    // Initialize the Turing machine tape
    tm->tape = tapeStart;

//     Check if all values are either zero or NULL character
//        if (transitionTable->writeSymbol == '\0' && transitionTable[currentState][(int)readSymbol].moveDirection == '\0' && transitionTable[currentState][(int)readSymbol].nextState == 0) {
//            continue; // Skip this line and continue with the next iteration
//        }
}

void runTM(struct TuringMachine* tm) {
    struct Cell* head = tm->tape;
    int currentState = tm->startState;
    int endState = tm->endState;
    struct Instruction (*transitionTable)[ASCII_RANGE] = tm->transitions;

    while (1) {
        char readSymbol = (head != NULL) ? head->value : 'B';

        // Check if instruction values are  zero, NULL
        if (transitionTable[currentState][(int)readSymbol].writeSymbol == '\0' && transitionTable[currentState][(int)readSymbol].moveDirection == '\0' && transitionTable[currentState][(int)readSymbol].nextState == 0) {
            break; // Skip this line and continue with the next iteration
        }

        // Check if a transition is defined for the current state and read symbol
        if (transitionTable[currentState][(int)readSymbol].nextState == endState) {
            break; // Exit the loop if no transition is defined
        }

        struct Instruction instruction = transitionTable[currentState][(int)readSymbol];

        if (head != NULL) {
            head->value = instruction.writeSymbol;
        }

        if (instruction.moveDirection == 'L') {
            head = moveLeft(head);
        } else if (instruction.moveDirection == 'R') {
            head = moveRight(head);
        }

        currentState = instruction.nextState;

        if (currentState == endState || head == NULL) {
            break;
        }
    }

    struct Cell* printHead = tm->tape;

    printf("Final tape contents: ");
    printTape(printHead);

}

void cleanUp(struct TuringMachine* tm) {
    struct Cell* tapeStart = tm->tape;
    while (tapeStart != NULL) {
        struct Cell* temp = tapeStart;
        tapeStart = tapeStart->next;
        free(temp);
    }
}

int main() {
    struct TuringMachine tm;
    char fileName[100];

    printf("Enter filename: ");
    scanf("%s", fileName);

    fillTuringMachine(&tm, fileName);

    runTM(&tm);

    cleanUp(&tm);

    return 0;
}