#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LENGTH 255
#define MAX_RECORDED_STEPS 100000
#define NUM_TAILS 9

struct Coords {

    int x; // Horizontal
    int y; // Vertical Axis

};

struct Rope {

    struct Coords Point;
    struct Rope *Tail;

};

// Recursive function to move the tails
void MoveTail(struct Rope *rope) {

    if (rope->Tail == NULL) {
        return; // Rope has no tail
    }

    int xDelta = rope->Point.x - rope->Tail->Point.x;
    int yDelta = rope->Point.y - rope->Tail->Point.y;

    // See if the tail is currently touching
    if ((xDelta >= -1 && xDelta <= 1) && (yDelta >= -1 && yDelta <= 1)) {
        // Tail is touching the head, do nothing
    } else if (xDelta == 0 && (yDelta > 1 || yDelta < -1)) {
        // Tail is in-line on the x axis, so move y 1

        if (yDelta > 0) {
            rope->Tail->Point.y++;
        } else {
            rope->Tail->Point.y--;
        }

    } else if (yDelta == 0 && (xDelta > 1 || xDelta < -1)) {
        // Tail is in-line on the y axis, so move y once

        if (xDelta > 0) {
            rope->Tail->Point.x++;
        } else {
            rope->Tail->Point.x--;
        }

    } else {

        // MoveStep 1 step diagnolly to keep up
        if (xDelta > 0) {
            rope->Tail->Point.x++;
        } else {
            rope->Tail->Point.x--;
        }

        if (yDelta > 0) {
            rope->Tail->Point.y++;
        } else {
            rope->Tail->Point.y--;
        }

    }

    MoveTail(rope->Tail);
    return;

}

void MoveStep(struct Rope *rope, char direction) {

    // Firstly, move the head as per the instruction
    switch (direction) {

        default:
            exit;
            break;
        case 'U':
            rope->Point.y++;
            // MoveStep Up
            break;
        case 'D':
            rope->Point.y--;
            // MoveStep Down
            break;
        case 'L':
            rope->Point.x--;
            // MoveStep Left
            break;
        case 'R':
            // MoveStep Right
            rope->Point.x++;
            break;
    }

    // Now we might move the tail
    MoveTail(rope);
    return;

}


int main(int argc, char *argv[]) {

    // Set up our rope with all its not

    struct Rope *ropeArray = calloc(NUM_TAILS + 1, sizeof(struct Rope));

    for (int i = 0; i <= NUM_TAILS; i++) {

        ropeArray[i].Point.x = 0;
        ropeArray[i].Point.y = 0;
        ropeArray[i].Tail = NULL;

        if (i > 0) {
            ropeArray[i - 1].Tail = &ropeArray[i];
        }

    }

    // Array to record each step the last tail trod on
    struct Coords *tailSteps = calloc(MAX_RECORDED_STEPS, sizeof(struct Coords));

    // Open the file for reading
    char buffer[BUFFER_LENGTH] = {0};
    FILE *inputFile = fopen(argv[1], "r");

    // Read the file
    int numSteps = 0;
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        char stepDirection = buffer[0];
        int stepQty = strtol(&buffer[2], NULL, 0);

        for (int i = 0; i < stepQty; i++) {

            // Make the move (One Step in the stepDirection)
            MoveStep(&ropeArray[0], stepDirection);

            struct Coords *tailStepsPtr = tailSteps + (numSteps * 1);

            *tailStepsPtr = ropeArray[NUM_TAILS].Point; // Get the current coords of the very last tail

            numSteps++;
        }

    }


    int uniqueCoords = 0;

    for (int i = 0; i < numSteps; i++) {

        int firstSeenCoord = 1;

        struct Coords *tailStepsPtr = tailSteps + (1 * i);

        int x = tailStepsPtr->x;
        int y = tailStepsPtr->y;

        // Now Dedupe

        for (int z = 0; z < i; z++) {

            struct Coords *tailStepsInnerPtr = tailSteps + (1 * z);

            if ((tailStepsInnerPtr->x == tailStepsPtr->x) && (tailStepsInnerPtr->y == tailStepsPtr->y)) {
                // Point item has already been seen
                firstSeenCoord = 0;
                break;
            }

        }

        if (firstSeenCoord) {
            uniqueCoords++;
        }

    }

    printf("Part 1: %i", uniqueCoords);
    free(tailSteps);
    return 0;
}
