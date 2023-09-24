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

// Function to create a new tape cell with the given value as a parameter
struct Cell* createCell(char value) {
    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
    if (newCell == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        exit(1);
    }
    newCell->value = value;
    newCell->prev = NULL;
    newCell->next = NULL;
    return newCell;
}


//struct Cell *head;

// Define the transition instruction structure
struct Instruction {
    char writeSymbol;
    char moveDirection;
    int nextState;
};

// Function to move the tape head one cell to the left
void moveLeft(struct Cell** tapeHead) {
    if (*tapeHead == NULL) {
        struct Cell *newCell = createCell('B');
        newCell->next = *tapeHead;
//        newCell->prev = NULL; // The new cell has no previous cell since it's the leftmost end
        *tapeHead = newCell;
    } else {
        *tapeHead = (*tapeHead)->prev; // Update the tape head to the previous cell
    }
}

// Function to move the tape head one cell to the right
void moveRight(struct Cell** tapeHead) {
    if (*tapeHead == NULL) {
        struct Cell* newCell = createCell('B');
        newCell->prev = *tapeHead;
//        newCell->next = NULL; // The new cell has no next cell since it's the rightmost end
    // Update the next cell's previous pointer
        *tapeHead = newCell;
    } else {
        *tapeHead = (*tapeHead)->next; // Update the tape head to the next cell
    }
}


// Function to print the content of the tape
void printTape(struct Cell* tapeHead) {
    struct Cell* current = tapeHead; // declare and initialize a new pointer 'current' to point to the same cell as 'head'
    while (current != NULL) {
        printf("%c", current->value); // Print the value of the current cell
        current = current->next; // Move to the next cell
    }
    printf("\n");
}


int main() {

    FILE* file;
    char fileName[100];

    printf("Enter filename: ");
    scanf("%s", fileName);

    file = fopen(fileName, "r");

    if (!file) {
        printf("Could not open file. Exiting...\n");
        exit(101);
    }

    char initialTape[MAX_TAPE_LENGTH];
    fgets(initialTape, sizeof(initialTape), file);
    initialTape[strlen(initialTape) - 1] = '\0'; // Remove newline character

    // Initialize the tape with the leftmost cell containing 'A'
    struct Cell* tapeStart = createCell('A');
    struct Cell* head = tapeStart;
//    int tapeIndex = 0;

//
//    int tapeIndex = 0;

    // Read and parse the input file
    int numStates, startState, endState;
    fscanf(file, "%d", &numStates);
    fscanf(file, "%d", &startState);
    fscanf(file, "%d", &endState);

    // Read and populate the transition table
    struct Instruction transitionTable[MAX_STATES][ASCII_RANGE];
    for (int state = 0; state < MAX_STATES; state++) {
        for (int symbol = 0; symbol < ASCII_RANGE; symbol++) {
            transitionTable[state][symbol].writeSymbol = 'B';
            transitionTable[state][symbol].moveDirection = 'L';
            transitionTable[state][symbol].nextState = -1;
        }
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int fromState, toState;
        char readVal, writeVal, moveDirection;

        if (sscanf(line, "(%d,%c)->(%c,%c,%d)", &fromState, &readVal, &writeVal, &moveDirection, &toState) == EOF) {
//            if (writeVal == '\0' || moveDirection == '\0' || toState == 0) {
//                continue;
//            }
            break;
        }

        transitionTable[fromState][(int)readVal].writeSymbol = writeVal;
        transitionTable[fromState][(int)readVal].moveDirection = moveDirection;
        transitionTable[fromState][(int)readVal].nextState = toState;
    }

    int currentState = startState;
    int tapeIndex = 0;

    // Append initialTape to the tape
    while (initialTape[tapeIndex] != '\0') {
        struct Cell* newCell = createCell(initialTape[tapeIndex]);
        head->next = newCell;
        newCell->prev = head;
        head = newCell;
        tapeIndex++;
    }

    printf("Writing tape...\n");
    printf("Initial tape contents: ");
    printTape(tapeStart);


//    // Append initialTape to the tape
//    while (initialTape[tapeIndex] != '\0') {
//        struct Cell* newCell = createCell(initialTape[tapeIndex]);
//        head->next = newCell;
//        newCell->prev = head;
//        head = newCell;
//        tapeIndex++;
//    }

    while (1) {

        char readSymbol = (head != NULL) ? head->value : 'B';

        struct Instruction instruction = transitionTable[currentState][(int)readSymbol];

        if (head != NULL) {
            head->value = instruction.writeSymbol;
        }

        if (instruction.moveDirection == 'L') {
            moveLeft(&head);
        } else if (instruction.moveDirection == 'R') {
            moveRight(&head);
        }

        currentState = instruction.nextState;

        if (currentState == -1) {
            break;
        }
    }

    // Print the final tape content
    printf("Final tape contents: ");
    printTape(tapeStart);

    // Clean up
    fclose(file);

    // Clean up tape memory
    while (tapeStart != NULL) {
        struct Cell* temp = tapeStart;
        tapeStart = tapeStart->next;
        free(temp);
    }

    return 0;
}