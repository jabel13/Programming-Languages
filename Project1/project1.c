#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of states
#define MAX_STATES 100

// Define the tape cell structure - this tape represents our linked data structure of cells
struct Cell {
    char value;
    struct Cell* next;
    struct Cell* prev;
};

// Function to create a new tape cell with the given value as a parameter
struct Cell* createCell(char value) {
    struct Cell* cell = (struct Cell*)malloc(sizeof(struct Cell)); // Allocate memory for the cell
    cell->value = value; // Set value of the cell - 1st character should be 'A'
    cell->next = NULL; // Initialize the next pointer to NULL
    cell->prev = NULL; // Initialize the next pointer to NULL
    return cell; // Return new cell
}

// - Structure for the tape head - allows the program to keep track of the current position on the tape
// - the tape head is the read/write head of the machine,
// it goes over a specific cell on the tape
// When pointing to a specific cell, update the head pointer to point to the next cell in the linked list
struct Cell *head;

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
        if (*tapeHead != NULL) {
            (*tapeHead)->prev = newCell; // Update the previous cell's next pointer
        }
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
        if (*tapeHead != NULL) {
            (*tapeHead)->next = newCell; // Update the next cell's previous pointer
        }
        *tapeHead = newCell;
    } else {
        *tapeHead = (*tapeHead)->next; // Update the tape head to the next cell
    }
}


// Function to print the content of the tape
void printTape(struct Cell* tapeHead) {
    struct Cell* current = tapeHead; // declare and initialize a new pointer 'current' to point to the same cell as 'head'
//    while (current != NULL) { // Loop through the tape until there's nothing there (NULL is reached)
//        printf("%c", current->value); // Print the value of the current cell
//        current = current->next; // Move to the next cell
//    }
//    printf("\n");
    while (current != NULL) {
        printf("%c", current->value); // Print the value of the current cell
        current = current->next; // Move to the next cell
    }
    printf("\n");
}


int main() {

    FILE* file;
    int numStates, startState, endState;
    char initialTape[100];
    char fileName[100];

    // Ask the user for the input file name
    printf("Enter the file name for the Turing Machine: ");
    scanf("%s", fileName);
//    file = fopen("/Users/jabelalcantara/Desktop/inputFile.txt", "r");
    file = fopen(fileName, "r");

    if (!file) {
        printf("Could not open file");
        exit(101);
    }

//    // Read and print the first line (Initial tape content)
//    fgets(initialTape, sizeof(initialTape), file);
//    printf("First line of input file: %s", initialTape);

    // Read initial tape content
    fgets(initialTape, sizeof(initialTape), file);
    initialTape[strlen(initialTape) - 1] = '\0'; // Remove newline character

    struct Cell *tapeStart = createCell('A');
//    struct Cell* head = tapeStart;
head = tapeStart;

// Append the initial tape contents to the right of 'A'
//    for (int i = 0; initialTape[i] != '\0'; i++) {
//        struct Cell *newCell = createCell(initialTape[i]);
//        head->next = newCell;
//        newCell->prev = head;
//        head = newCell;
//    }

//    head = tapeStart; // Reset local pointer


//
//    // Read number of states, start state, and end state
    fscanf(file, "%d", &numStates);
//    printf("%d ", numStates);
    fscanf(file, "%d", &startState);
//    printf("%d ", startState);
    fscanf(file, "%d", &endState);
//    printf("%d ", endState);


    // Define the transition table
//    struct Instruction transitionTable[MAX_STATES][128] = {0};
//    int currentState = startState;

//    head = createCell('A');
//
//    // Move the tape head to the leftmost cell ('A')
//    while (head->next != NULL) {
//        head = head->next;
//    }

    // Append the initial tape contents to the right of 'A'
//    for (int i = 0; initialTape[i] != '\0'; i++) {
//        struct Cell* newCell = createCell(initialTape[i]);
//        head->next = newCell;
//        head = newCell;
//    }
    struct Instruction transitionTable[MAX_STATES][128] = {0};
    // Read and parse the transition instructions
    char line[100];
    int skipInitialInstructions = 1; // Flag to skip initial instructions

    while (fgets(line, sizeof(line), file)) {
        int fromState, toState;
        char readVal, writeVal, moveDirection;

        if (skipInitialInstructions) {
            if (sscanf(line, "(0,%c)->(%c,%c,%d)\n", &readVal, &writeVal, &moveDirection, &toState) == 4) {
                // Skip initial instructions
                continue;
            } else {
                skipInitialInstructions = 0; // Turn off skipping
            }
        }

        if (sscanf(line, "(%d,%c)->(%c,%c,%d)\n", &fromState, &readVal, &writeVal, &moveDirection, &toState) == 5 ) {
        // Store the instruction in the transition table
        transitionTable[fromState][(int)readVal].writeSymbol = writeVal;
        transitionTable[fromState][(int)readVal].moveDirection = moveDirection;
        transitionTable[fromState][(int)readVal].nextState = toState;
        }

        printf("(%d,%c)->(%c,%c,%d)\n", fromState, readVal, writeVal, moveDirection, toState);
    }

    printf("Writing tape...\n");
    printf("Initial tape contents: ");
    printTape(tapeStart);

    int currentState = startState; // Set the initial state to 0
    int tapeIndex = 0;


//
    // Run the Turing machine
//    int currentState = startState; // Set the initial state
    while (currentState != endState) {
        char readSymbol = head->value;
        struct Instruction instruction = transitionTable[currentState][(int)readSymbol];

        head->value = instruction.writeSymbol;

        if (instruction.moveDirection == 'L') {
            moveLeft(&head);
        } else if (instruction.moveDirection == 'R') {
            moveRight(&head);
        }

        currentState = instruction.nextState;
        // Print current state, tape, and action
        printf("State %d\n", currentState);
        printTape(tapeStart);
        printf("\n");
        printf("Write %c, Move %c, New State %d\n", instruction.writeSymbol, instruction.moveDirection, currentState);
        printf("------------\n");

        if (instruction.moveDirection == 'R') {
            tapeIndex++; // Move the tape index to the right
            if (tapeIndex >= strlen(initialTape)) {
                // If the tape index goes beyond the initial tape content, append a blank symbol ('B')
                initialTape[tapeIndex] = 'B';
                initialTape[tapeIndex + 1] = '\0'; // Null-terminate the string
            }
        } else if (instruction.moveDirection == 'L' && tapeIndex > 0) {
            tapeIndex--; // Move the tape index to the left
        }
    }

    // Print the final tape content
    printf("Final tape contents: ");
    printTape(tapeStart);

    // Clean up
    fclose(file);


    // Clean up tape memory
    while (tapeStart != NULL) {
        struct Cell *temp = tapeStart;
        tapeStart = tapeStart->next;
        free(temp);
    }



    return 0;
}
