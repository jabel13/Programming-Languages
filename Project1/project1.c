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
    struct Cell* tape; // Pointer to the tape
    int startState; // Start state
    int endState;   // End state
    struct Instruction** transitions;   // 2D array to represent instructions
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

// fill in values from input file & print initial tape contents
struct TuringMachine fillTuringMachine (const char* fileName) {
    FILE* file;

    file = fopen(fileName, "r");

    if (!file) {
        printf("Could not open file. Exiting...\n");
        exit(1);
    }

    struct TuringMachine tm;

    char initialTape[MAX_TAPE_LENGTH];
    fgets(initialTape, sizeof(initialTape), file);
    initialTape[strlen(initialTape) - 1] = '\0'; // Remove newline character

    // Initialize the tape with the leftmost cell containing 'A'
    struct Cell* tapeStart = (struct Cell*)malloc(sizeof(struct Cell));
    tapeStart->value = 'A';
    tapeStart->next = NULL;
    tapeStart->prev = NULL;

    struct Cell* head = tapeStart;

    // Read and parse the input file
    int numStates, startState, endState;
    fscanf(file, "%d", &numStates);
    fscanf(file, "%d", &startState);
    fscanf(file, "%d", &endState);

    // Allocate memory for the TM transitions
    // It allocates memory for rows of instructions (One row per state, and for columns (ASCII characters)
    tm.transitions = (struct Instruction**) malloc(MAX_STATES * sizeof(struct Instruction*)); // tm.transitions is a pointer that points to an array of pointers, which will point to a row of instructions
    for (int i = 0; i < MAX_STATES; i++) {
        tm.transitions[i] = (struct Instruction*)malloc(ASCII_RANGE * sizeof(struct Instruction));
    }


    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int fromState, toState;
        char readVal, writeVal, moveDirection;

        if (sscanf(line, "(%d,%c)->(%c,%c,%d)", &fromState, &readVal, &writeVal, &moveDirection, &toState) != 5 || (fromState == 0 && toState == 0 && readVal == '\0' && writeVal == '\0' && moveDirection == '\0')) {
            break;
        }

        // Having a problem with the values that were being stored so
        // Check if all values are either zero or NULL character
        if (fromState == 0 && toState == 0 && readVal == '\0' && writeVal == '\0' && moveDirection == '\0') {
            continue; // Skip this line and continue with the next iteration
        }

        // Another issue with storing the instructions
        // If these values are set to these variables - don't store them in thhe instructions
        if (!(writeVal == '\0' && moveDirection == '\0' && toState == 0)) {
            // Store the transition in the 2D array
            tm.transitions[fromState][(int)readVal].writeSymbol = writeVal;
            tm.transitions[fromState][(int)readVal].moveDirection = moveDirection;
            tm.transitions[fromState][(int)readVal].nextState = toState;
        }
    }

    fclose(file);

    // to keep track of current position in the initial tape
    int tapeIndex = 0;

    // Allocate cells on-the-fly for the input tape
    while (initialTape[tapeIndex] != '\0') {
        struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
        newCell->value = initialTape[tapeIndex];
        newCell->next = NULL;
        newCell->prev = head;
        head->next = newCell;
        head = newCell;
        tapeIndex++;
    }

    printf("Initial tape contents: ");
    printTape(tapeStart);

    // Initialize the Turing machine tape
    tm.tape = tapeStart;

    tm.startState = startState;
    tm.endState = endState;



    return tm;

}

void runTM(struct TuringMachine* tm) {
    // initialize tape head to beginning of the tape
    struct Cell* head = tm->tape;
    // Get currentState & endState from the TM
    int currentState = tm->startState;
    int endState = tm->endState;
    // Get transition table
    struct Instruction** transitions = tm->transitions;


    // This is the main loop fro the Turing Machine
    while (currentState != endState && head != NULL) {
        // readSymbol determines the symbol read from the tape, which is used to look up instructions in the transition table
        char readSymbol = head->value;

        // Grab the instruction for the current state and read symbol from the transition table
        struct Instruction instruction = transitions[currentState][(int)readSymbol];

        if (!(instruction.writeSymbol == '\0' && instruction.moveDirection == '\0' && instruction.nextState == 0)) {
            // update the tape cell with the symbol to write
            if (head != NULL) {
                head->value = instruction.writeSymbol;
            }

            //Move tape left  or right based on instruction
            if (instruction.moveDirection == 'L') {
                head = moveLeft(head);
            } else if (instruction.moveDirection == 'R') {
                head = moveRight(head);
            }

            // update current state to the next state
            currentState = instruction.nextState;
        } else {
            break; // Halt TM when no valid transition is found
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

    // Free the allocated memory for the transitions 2D array
    for (int i = 0; i < MAX_STATES; i++) {
        free(tm->transitions[i]);
    }
    free(tm->transitions);
}

int main() {
    char fileName[100];

    printf("Enter filename: ");
    scanf("%s", fileName);

    struct TuringMachine tm = fillTuringMachine(fileName);

    runTM(&tm);

    cleanUp(&tm);

    return 0;
}