#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of states
#define MAX_STATES 100

// Define the tape cell structure
struct Cell {
    char value;
    struct Cell* next;
};

// Function to create a new tape cell with the given value
struct Cell* createCell(char value) {
    struct Cell* cell = (struct Cell*)malloc(sizeof(struct Cell)); // Allocate memory for the cell
    cell->value = value; // Set value of the cell
    cell->next = NULL; // Initialize the next pointer to NULL
    return cell; // Return new cell
}

// Structure for the tape head - allows the program to keep track of the current position on the tape
struct Cell* head = NULL; // Declare a pointer to the tape head

// Define the transition instruction structure
struct Instruction {
    char writeSymbol;
    char moveDirection;
    int nextState;
};

// Function to move the tape head one cell to the left
void moveLeft() {
    struct Cell* newCell = createCell('A'); // Creates a new cell with 'A'
    newCell->next = head; // set the next pointer of the new cell to the current head
    head = newCell; // Update the head to the new cell
}

// Function to print the content of the tape
void printTape() {
    struct Cell* current = head;
    while (current != NULL) { // Loop through the tapee until the end (NULL is reached)
        printf("%c", current->value); // Print the value of the current cell
        current = current->next; // Move to the next cell
    }
    printf("\n");
}


int main() {
    // Coming soon...
    return 0;
}
