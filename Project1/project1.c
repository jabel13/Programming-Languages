#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of states
#define MAX_STATES 100

// Define the tape cell structure - this tape represents our linked data structure of cells
struct Cell {
    char value;
    struct Cell* next;
};

// Function to create a new tape cell with the given value as a parameter
struct Cell* createCell(char value) {
    struct Cell* cell = (struct Cell*)malloc(sizeof(struct Cell)); // Allocate memory for the cell
    cell->value = value; // Set value of the cell - 1st character should be 'A'
    cell->next = NULL; // Initialize the next pointer to NULL
    return cell; // Return new cell
}

// - Structure for the tape head - allows the program to keep track of the current position on the tape
// - the tape head is the read/write head of the machine,
// it goes over a specific cell on the tape
// When pointing to a specific cell, update the head pointer to point to the next cell in the linked list
struct Cell* head = NULL; // set to 'NULL' because it doesn't currently point to any cell on the tape

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
    head = newCell; // Update the tape head to the new cell
}

// Function to print the content of the tape
void printTape() {
    struct Cell* current = head; // declare and initialize a new pointer 'current' to point to the same cell as 'head'
    while (current != NULL) { // Loop through the tape until there's nothing there (NULL is reached)
        printf("%c", current->value); // Print the value of the current cell
        current = current->next; // Move to the next cell
    }
    printf("\n");
}


int main() {

    FILE* file;
    int numStates;

    file = fopen("/Users/jabelalcantara/Desktop/inputFile.txt", "r");

    if (!file) {
        printf("Could not open file");
        exit(101);
    }

    while((fscanf(file, "%d", &numStates)) == 1) {
        printf("%d ", numStates);
    }



    return 0;
}
