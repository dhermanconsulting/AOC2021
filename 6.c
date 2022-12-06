#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 4096

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Missing input\n");
        return -1;
    }

    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

    char buffer[BUFFER_LENGTH] = {0};

    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        unsigned long  lineLength = strlen(buffer);

        // Part 1 (Very lazy)
        for(int sequenceEndChar = 3; sequenceEndChar < lineLength; sequenceEndChar++ ){

            char char1 = buffer[sequenceEndChar-3];
            char char2 = buffer[sequenceEndChar-2];
            char char3 = buffer[sequenceEndChar-1];
            char char4 = buffer[sequenceEndChar-0];

            if(char1 == char2 || char1 == char3 || char1 == char4) continue;

            if(char2 == char3 || char2 == char4) continue;

            if(char3 == char4) continue;

            int markerChar = sequenceEndChar + 1;

            printf("%i\n", markerChar);
            break;

        }

        // Part 2 - somewhat more expandable, though unoptimised!
        for(int sequenceEndChar = 13; sequenceEndChar < lineLength; sequenceEndChar++) {

            //nextInSequence:;

            char messageMarkers[14] = {0};
            int charArrayIndex = 0;

            // Load the characters into the array
            for(int charIndex = (sequenceEndChar - 13); charIndex <= sequenceEndChar; charIndex++) {

                char character = buffer[charIndex];
                messageMarkers[charArrayIndex] = character;

                charArrayIndex++;

            }

            // Find matching letters

            int dupes = 0;

            for(int i = 0; i < 14; i++){

                for(int x = 0; x < 14; x++) {

                    if(i == x) continue;

                    if(messageMarkers[i] == messageMarkers[x]) {
                        dupes = 1;
                    }

                }

            }

            if(dupes == 1) continue;
            int markerChar = sequenceEndChar + 1;
            printf("%i", markerChar);
            break;
        }


    }

    return 0;
}
