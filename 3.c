#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GetValue(char letter) {
    if((int) letter > 96) {
        // Letter is lowercase
        return letter - 96;
    }else{
        // Letter is uppercase
        return (letter - 65) + 27;
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Missing input\n");
        exit(-1);
    }

    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

    if (inputFile == NULL)
        exit(-1);

    char buffer[255] ={0};
    char lines[500][255];
    int currentLine = 0;
    int totalScore = 0;

    while(fgets(buffer, sizeof(buffer), inputFile) != NULL){

        memcpy(lines[currentLine], buffer, sizeof(buffer));

        // Get the lenght of the string
        int lineLength = strlen(buffer);

        // Process the line
        for(int i = 0; i < (lineLength / 2); i++){

            // Second half iteration
            for(int x = (lineLength / 2); x < lineLength; x++) {

                if(buffer[i] ==  buffer[x]) {

                    totalScore += GetValue(buffer[x]);;

                    // Stop processing the line
                    goto nextLine;

                }
            }
        }

        nextLine:;
        currentLine++;
    }

    // Second part

    int pt2Total = 0;
    int numGroups = (currentLine + 1) / 3;

    for(int i = 0; i < numGroups; i++) {

        int startLine = i * 3;

        int l1length = strlen(lines[startLine]);
        int l2length = strlen(lines[startLine + 1]);
        int l3length = strlen(lines[startLine + 2]);

        // Iterate through each character in line 1 and find which char appears in the other lines too
        for (int l1c = 0; l1c < l1length; l1c++) {

            char l1Char = lines[startLine][l1c];

            // Test Line 2
            for (int l2c = 0; l2c < l2length; l2c++) {

                char l2Char = lines[startLine + 1][l2c];

                if (l1Char != l2Char) continue;

                // Found in line 2, test line 3
                for(int l3c = 0; l3c < l3length; l3c++) {

                    char l3Char = lines [startLine+2][l3c];

                   if(l1Char != l3Char) continue;

                   // Character exists in all three
                   int score = GetValue(l1Char);
                   pt2Total += score;

                   goto nextGroup;
                }

            }

        }
        nextGroup:;
    }

    printf(" Part 1 Total: %i\n", totalScore);
    printf(" Part 2 Total: %i\n", pt2Total);
    return 0;
}
