#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_LENGTH 2048

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Missing input\n");
        return -1;
    }

    // Open the file for reading
    char buffer[BUFFER_LENGTH] = {0};
    FILE *inputFile = fopen(argv[1], "r");

    int totalRows = 0;
    int totalCols = 0;

    // Initial scan of the file so we can set up the array
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        unsigned long lineLength = strlen(buffer) - 1; // -1 to ignore newlines

        if (lineLength <= 0) continue;

        if (lineLength > totalCols) {
            totalCols = lineLength; // Ignore newlines
        }

        totalRows++;
    }

    // Set up our 2D Array
    int *treeGrid = calloc(totalRows, sizeof(int) * totalCols);

    // Rewind the file
    rewind(inputFile);

    int currentRow = 0;

    // Go through the file again and import
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        unsigned long lineLength = strlen(buffer) - 1;

        if (lineLength > totalCols) { return 99; } // Whoops

        int *linePointer =
                treeGrid + (currentRow * totalCols); // Set the pointer to the start of our line in the 2D Array

        for (int i = 0; i < lineLength; i++) {

            *(linePointer + i) = buffer[i] - '0';

        }

        currentRow++;
    }

    // We now have a 2D array of Rows * Columns representing the trees
    // Part 1, count the visible trees in the middle

    int part1Answer = 0;

    // Start by working out the trees round the edge
    int edgeTrees = (totalCols * 2) + (totalRows * 2) - 4;

    part1Answer += edgeTrees;

    // Go through each interior tree in turn
    for (int targetRow = 1; targetRow < (totalRows - 1); targetRow++) {

        for (int targetColumn = 1; targetColumn < (totalCols - 1); targetColumn++) {

            // Only look at the interior trees
            int *targetPointer =
                    treeGrid + (targetRow * totalCols) + targetColumn; // Pointer to targetTree we're looking at
            int targetTree = *targetPointer;

            // For each targetTree, see if there is another targetTree between it and the edge that's higher

            int visible = 1;

            // See if the tree is the tallest in this column

            for (int checkRow = 0; checkRow < totalRows; checkRow++) {

                int *treePointer = treeGrid + (checkRow * totalCols) + targetColumn;
                int treeHeight = *treePointer;

                if (targetPointer == treePointer) {
                    // This is our own targetTree, if we're still visible now then we can be seen from that direction
                    if (visible == 1) break;
                    visible = 1;
                    continue; // Don't count our own tree
                }

                if (treeHeight >= targetTree) {
                    // This tree masks ours from this direction
                    visible = 0;
                }

            }

            if (visible == 1) {
                // Tree can be seen, count and move on
                part1Answer++;
                // printf("Tree at %i,%i (Height %i) SEEN.\n", (targetRow + 1), (targetColumn + 1), targetTree);
                continue;
            }

            visible = 1;

            // Now check our targets row
            for (int checkCol = 0; checkCol < totalCols; checkCol++) {

                int *treePointer = treeGrid + (targetRow * totalCols) + checkCol;
                int treeHeight = *treePointer;

                if (targetPointer == treePointer) {
                    // This is our own targetTree, if we're visible now then we can be seen from that direction
                    if (visible == 1) break;
                    visible = 1;
                    continue; // Don't count our own tree
                }

                if (treeHeight >= targetTree) {
                    // This tree masks ours from this direction
                    visible = 0;
                };

            }

            if (visible == 1) {
                // Tree can be seen, count and move on
                part1Answer++;
                // printf("Tree at %i,%i (Height %i) SEEN.\n", (targetRow + 1), (targetColumn + 1), targetTree);
                continue;
            }

            // printf("Tree at %i,%i (Height %i) NOT seen.\n", (targetRow + 1), (targetColumn + 1), targetTree);

        }

    }

    // Part 2

    int part2Answer = 0;

    // Go through each tree in turn
    for (int targetRow = 0; targetRow < (totalRows - 0); targetRow++) {

        for (int targetColumn = 0; targetColumn < (totalCols - 0); targetColumn++) {

            int *targetPointer =
                    treeGrid + (targetRow * totalCols) + targetColumn; // Pointer to targetTree we're looking at
            int targetTree = *targetPointer;

            // Distances to the edges (i.e., possible max vis for the tree)
            int northTrees = targetRow;
            int southTrees = totalRows - targetRow - 1;
            int eastTrees = targetColumn;
            int westTrees = totalCols - targetColumn - 1;

            // Walk North
            for (int checkRow = targetRow; checkRow >= 0; checkRow--) {

                int *treePointer = treeGrid + (checkRow * totalCols) + targetColumn;
                int treeHeight = *treePointer;

                if (targetPointer == treePointer) {
                    continue; // Ignore our target tree
                }

                if (treeHeight >= targetTree) {
                    // This tree is the same or taller
                    northTrees = targetRow - checkRow;
                    break;
                }
            }

            // Walk South
            for (int checkRow = targetRow; checkRow < totalRows; checkRow++) {

                int *treePointer = treeGrid + (checkRow * totalCols) + targetColumn;
                int treeHeight = *treePointer;

                if (targetPointer == treePointer) {
                    continue; // Ignore our target tree
                }

                if (treeHeight >= targetTree) {
                    southTrees = checkRow - targetRow;
                    break;
                }

            }

            // Walk East

            // Now check our targets row
            for (int checkCol = targetColumn; checkCol >= 0; checkCol--) {

                int *treePointer = treeGrid + (targetRow * totalCols) + checkCol;
                int treeHeight = *treePointer;

                if (targetPointer == treePointer) {
                    continue; // Ignore our target tree
                }

                if (treeHeight >= targetTree) {
                    // This tree is the same or taller
                    eastTrees = targetColumn - checkCol;
                    break;
                }

            }

            // Walk West
            for (int checkCol = targetColumn; checkCol < totalCols; checkCol++) {

                int *treePointer = treeGrid + (targetRow * totalCols) + checkCol;
                int treeHeight = *treePointer;

                if (targetPointer == treePointer) {
                    continue; // Ignore our target tree
                }

                if (treeHeight >= targetTree) {
                    // This tree is the same or taller

                    // West calc, how far from us is this tree
                    westTrees = checkCol - targetColumn;
                    break;

                }

            }

            int scenicScore = northTrees * eastTrees * southTrees * westTrees;
            if (part2Answer < scenicScore) part2Answer = scenicScore;

            // printf("Tree %i, %i score: %i\n", targetColumn, targetRow, scenicScore);

        }

    }

    printf("Part 1: %i\n", part1Answer);
    printf("Part 2: %i\n", part2Answer);

    return 0;
}
