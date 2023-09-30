#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// This is the item struct, it represents the name and price of a single item inside a 2D shelving unit (array)
struct item {
    char name[20];
    float price;
};

// This is a boolean function to check if a slot is occupied
// Parameters are a struct 'shelfArray', and two integers 'shelf' & 'slot' (row & columns)
bool isSlotOccupied(struct item **shelfArray, int shelf, int slot) {
    // Returns TRUE when a slot is filled - first character of the item's name is not the null terminator - where the string ends
    // Returns FALSE when a slot is empty - first character of the item's name is the null terminator
    return shelfArray[shelf - 1][slot - 1].name[0] != '\0';
}

// Function to get item details
void itemDetails(struct item **shelfArray, int numOfShelves, int numOfSlots) {
    // This initializes an array of characters itemDetails to 100, it the holds description of entire item information
    char itemDetails[100];

    // Simple instructions for the user
    printf("Add item details in this format: <name>, <price>, <shelf>, <slot>"
           "\nFor instance: book, 15.50, 2,3"
           "\nEnter 'q' to exit the program."
           "\nEnter 'd' to finish adding items.\n\n");

    // Infinite loop until user enters 'q' - quits program or 'd' - exits the loop
    do {
        printf("Enter item details: ");

        // Grab the user input
        scanf(" %99[^\n]", itemDetails);

        // If the firat character of the itemDetails array is q, quit the program
        if (itemDetails[0] == 'q') {
            exit(0);
        }
            // If the first character is 'd', exit loop
        else if (itemDetails[0] == 'd') {
            break;
        }

        char name[20];
        float price;
        int shelf, slot;

        // Parse user input
        // Extract values based on this specific format
        // 'name' reads up to 19 characters, [^,] disregards a comma
        // Read all four values
        if (sscanf(itemDetails, "%19[^,], %f, %d, %d", name, &price, &shelf, &slot) == 4) {
            // Check if the shelf and slot are valid
            if (shelf >= 1 && shelf <= numOfShelves && slot >= 1 && slot <= numOfSlots) {
                // Add an item to a slot if its empty, so negate the return value of the function
                if (!isSlotOccupied(shelfArray, shelf, slot)) {
                    // Add the item to the specified shelf and slot
                    strcpy(shelfArray[shelf - 1][slot - 1].name, name);
                    shelfArray[shelf - 1][slot - 1].price = price;
                    printf("Item added to shelf %d, slot %d\n", shelf, slot);
                } else {
                    // Slot is occupied
                    printf("Shelf %d, slot %d is already occupied.\n", shelf, slot);
                }
            } else {
                // Shelf or slot values are out of range
                printf("Invalid shelf or slot. Please enter valid values.\n");
            }
        } else {
            // Invalid input
            printf("Please use the format <name>, <price>, <shelf>, <slot>\n");
        }
        // Clear the input buffer
        while (getchar() != '\n');
    } while (1);
}

// Function to look an item up
void lookItemUp(struct item **shelfArray, int numOfShelves, int numOfSlots) {

    do {
        int shelf, slot;

        // Allow user to enter shelf and slot coordinate pairs
        printf("Enter shelf and slot coordinates to look up an item (i.e., 2,1): ");

        // Checki if the input contains two integer values, seperated by a comma
        if(scanf("%d,%d", &shelf, &slot) == 2) {
            // Check if the values are within valid ranges
            if (shelf>= 1 && shelf <= numOfShelves && slot >= 1 && slot <= numOfSlots) {
                // Check if the slot is occupied
                if (isSlotOccupied(shelfArray, shelf, slot)) {
                    // Retrieve the item information and display it to the user
                    struct item item = shelfArray[shelf - 1][slot - 1];
                    printf("Name: %s, Price: %.2f\n", item.name, item.price);
                } else {
                    // Slot is empty
                    printf("Empty slot! Try again. \n");
                }
            } else {
                // Invalid values
                printf("Invalid shelf or slot. Please enter valid values.\n");
            }
        } else if (getchar() == 'q') {
            break; // Exit loop when q is entered
        } else {
            printf("Invalid input. Please enter value that follow the 'shelf,slot' format.\n");
        }
        // Clear the input buffer
        while (getchar() != '\n');
    } while (1);
}


int main() {

    // Declare variables that represent the rows and columns of a 2D structure
    // Shelves = rows and slots = columns
    int numOfShelves, numOfSlots;

    // Receive user input
    printf("Enter number of shelves: ");
    scanf("%d", &numOfShelves);
    printf("Enter number of slots available on each shelf: ");
    scanf("%d", &numOfSlots);


    // This dynamically allocates memory for a two-dimensional array of struct item pointers from user-entered values
    // and declares a pointer to a pointer to a struct item
    // Each pointer in 'shelfArray' represents a shelf that will point to an array of
    // struct item elements
    struct item **shelfArray = (struct item **)malloc(numOfShelves * sizeof(struct item *));

    // Loop through each shelf in the shelfArray and initiate each shelf
    // Create one shelf per iteration
    for (int i = 0; i < numOfShelves; i++) {
        // This allocates memory for a single shelf, which is an array of item elements
        shelfArray[i] = (struct item *) malloc(numOfSlots * sizeof(struct item));
    }

    // Set up item details based on shelfArray, numOfShelves, and numOfSlots
    itemDetails(shelfArray, numOfShelves, numOfSlots);
    // Look an item up based on shelfArray, numOfShelves, and numOfSlots
    lookItemUp(shelfArray, numOfShelves, numOfSlots);

    // Free dynamically allocated memory for each shelf
    for (int i = 0; i < numOfShelves; i++) {
        free(shelfArray[i]);
    }

    // Free dynamically allocated memory for the shelfArray itself
    free(shelfArray);

    return 0;
}
