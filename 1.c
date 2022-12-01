#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    assert(argc == 2 && "Must provide an argument => input.txt path");

    printf("Input file: %s\n", argv[1]);

    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

        if (inputFile == NULL)
        exit(EXIT_FAILURE);

    // Read in the file and calories

    char buffer[255];

    // Zero Array
    int i;
    for(i = 0; i < 255; i++){
        buffer[i] = 0;
    }

    // First Puzzle
    int maxCalories = 0;
    int currentElfCalories = 0;
    while(fgets(buffer, sizeof(buffer), inputFile) != NULL){

        if(buffer[0] == (char) '\n') {

            if(currentElfCalories > maxCalories) {
                maxCalories = currentElfCalories;
            }

            currentElfCalories = 0;
        }

        long int line = strtol(buffer, NULL, 0);

        currentElfCalories += line;

    }

    // maxCalories contains the answer

        return 0;
}
