#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int comp (const void *a,const void *b)
{

    int x = (*(int *)a);
    int y = (*(int *)b);

    return (y-x);
}

int main(int argc, char *argv[]) {

    assert(argc == 2 && "Must provide an argument => input.txt path");

    printf("Input file: %s\n", argv[1]);

    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

        if (inputFile == NULL)
        exit(EXIT_FAILURE);

    // Read in the file and calories

    char buffer[255] ={0};

    int allTotals[5000] = {0};

    int currentElf = 0;
    int currentElfTotal = 0;

    while(fgets(buffer, sizeof(buffer), inputFile) != NULL){

        if(buffer[0] == (char) '\n') {

            allTotals[currentElf] = currentElfTotal;

            currentElfTotal = 0; // Reset the current elf
            currentElf++;// Increment the currentElf pointer
        }

        int line = (int) strtol(buffer, NULL, 0);

        currentElfTotal += line;

    }

    fclose(inputFile);

    int numElements = sizeof(allTotals)/sizeof(*allTotals);

    qsort(allTotals, numElements, sizeof(int), comp);

    printf("The elf with the most calories has: %i\n", allTotals[0]);

    int showTop = 3;
    int topTotal = 0;

    int i = 0;
    for(i = 0; i < showTop; i++) {

        topTotal += allTotals[i];

    }

    printf("The top %i elves are carrying a total of %i\n", showTop, topTotal);

    return 0;
}
