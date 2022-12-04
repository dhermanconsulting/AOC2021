#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Missing input\n");
        exit(-1);
    }

    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

    if (inputFile == NULL)
        exit(-1);

    char buffer[255] = {0};

    int Part1Answer = 0;
    int Part2Answer = 0;

    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        // Get the length of the string
        unsigned long lineLength = strlen(buffer);

        int elfRange[4] = {0};
        int currentIndex = 0;

        char *startPointer = &buffer;
        char *endPointer = &buffer[lineLength];

        while (startPointer < endPointer) {
            char lineChar = *startPointer;

            if (lineChar == '\n') {
                break;
            }

            if (lineChar == '-') {
                startPointer++;
                continue;
            }

            if (lineChar == ',') {
                startPointer++;
                continue;
            }

            char *currentBufferPtr = startPointer;
            char *nextPointer = startPointer;

            int number = strtol(currentBufferPtr, &nextPointer, 0);

            elfRange[currentIndex] = number;
            currentIndex++;

            startPointer = nextPointer;
        }

        if (elfRange[0] >= elfRange[2] && elfRange[1] <= elfRange[3]) {
            // First range entirely within second

            //printf("%i - %i exists within %i - %i\n", elfRange[0], elfRange[1], elfRange[2], elfRange[3]);
            Part1Answer++;
        } else if (elfRange[2] >= elfRange[0] && elfRange[3] <= elfRange[1]) {
            //printf("%i - %i exists within %i - %i\n", elfRange[2], elfRange[3], elfRange[0], elfRange[1]);
            Part1Answer++;

        }

        // Part 2

        // Test for any overlap
        if (elfRange[0] >= elfRange[2] && elfRange[0] <= elfRange[3]) {
            // Partial overlap
            Part2Answer++;

        } else if (elfRange[2] >= elfRange[0] && elfRange[2] <= elfRange[1]) {
            // Partial overlap
            Part2Answer++;

        }

    }
    
    printf("Part 1 Answer: %i!\n", Part1Answer);
    printf("Part 2 Answer: %i\n", Part2Answer);
    return 0;
}
