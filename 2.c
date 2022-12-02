#include <stdio.h>
#include <stdlib.h>

enum roundResult {
    left = 0,
    right = 1,
    draw = -1
};

enum move {
    rock = 0,
    paper = 1,
    scissors = 2
};

enum move decodeMove(char encryptedMove, int right) {

    enum move result;

    if(right != 0) {
        // This move is a right hand column move
        switch (encryptedMove) {
            case 'X':
                return rock;

            case 'Y':
                return paper;

            case 'Z':
                return scissors;
        }

        exit(EXIT_FAILURE);
    }

    // This move is a left hand column move
    switch (encryptedMove) {
        case 'A':
            return rock;

        case 'B':
            return paper;

        case 'C':
            return scissors;
    }

    exit(EXIT_FAILURE);

}


enum roundResult playGame(enum move leftMove, enum move rightMove) {

    // Let's play rock paper scissors

    switch(leftMove) {

        case rock:
            switch(rightMove){
                case rock:
                    return draw; // Draw
                case paper:
                    return right; // Right wins
                case scissors:
                    return left; // Left wins
            }
            break;

        case paper:
            switch(rightMove){
                case rock:
                    return left; // Left wins
                case paper:
                    return draw; // Draw
                case scissors:
                    return right; // Right wins
            }
            break;

        case scissors:
            switch(rightMove){
                case rock:
                    return right; // Left wins
                case paper:
                    return left; // Right wins
                case scissors:
                    return draw; // Draw
            }
            break;
    }

    exit(EXIT_FAILURE);


}

int calculateScore(enum roundResult result, enum move playedMove, int player) {

    int score = 0;

    switch(playedMove){

        case rock:
            score = 1;
            break;
        case paper:
            score = 2;
            break;
        case scissors:
            score = 3;
            break;
    }

    if(result == draw){
        score += 3;
        return score;
    }

    if(player == result) {
        score += 6;
        return score;
    }

    return score;


}

enum move GetNextMove(enum move leftMove, char desiredResultEnc) {

    enum roundResult desiredResult;

    switch(desiredResultEnc){
        case 'X':
            desiredResult = left;
            break;

        case 'Y':
            desiredResult = draw;
            break;

        case 'Z':
            desiredResult = right;
            break;
    }

    switch(leftMove) {


        case rock:

            switch(desiredResult){
                case left:
                    return scissors; // Left wins
                case right:
                    return paper; // Right wins
                case draw:
                    return rock; // Draw
            }

            break;
        case paper:

            switch(desiredResult){
                case left:
                    return rock; // Left wins
                case right:
                    return scissors; // Right wins
                case draw:
                    return paper; // Draw
            }

            break;

        case scissors:

            switch(desiredResult){
                case left:
                    return paper; // Left wins
                case right:
                    return rock; // Right wins
                case draw:
                    return scissors; // Draw
            }

            break;
    }


}


int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Missing input\n");
        exit(EXIT_FAILURE);
    }

    long leftScoreTotal = 0;
    long rightScoreTotal = 0;

    long leftScoreTotalPt2 = 0;
    long rightScoreTotalPt2 = 0;


    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

    if (inputFile == NULL)
        exit(EXIT_FAILURE);

    char buffer[255] ={0};

    while(fgets(buffer, sizeof(buffer), inputFile) != NULL){

        // String is in the format "a b", so we need to split to find the first move, and response
        char leftChar = buffer[0];
        char rightChar = buffer[2];

        enum move leftMove = decodeMove(leftChar, 0);
        enum move rightMove = decodeMove(rightChar, 1);

        // Play the round
        enum roundResult result = playGame(leftMove, rightMove);

        // Calculate each players score
        int leftScore = calculateScore(result, leftMove, 0);
        int rightScore = calculateScore(result, rightMove, 1);

        // Total up
        leftScoreTotal += leftScore;
        rightScoreTotal += rightScore;


        // Part two solutions
        enum move nextMove = GetNextMove(leftMove, rightChar);

        // Play the round
        enum roundResult resultPt2 = playGame(leftMove, nextMove);

        // Calculate each players score
        int leftScorePt2 = calculateScore(resultPt2, leftMove, 0);
        int rightScorePt2 = calculateScore(resultPt2, nextMove, 1);

        // Total up
        leftScoreTotalPt2 += leftScorePt2;
        rightScoreTotalPt2 += rightScorePt2;


    }
    printf("Total: Left: %i Right: %i\n", leftScoreTotal, rightScoreTotal);
    printf("Total Part 2: Left: %i Right: %i\n", leftScoreTotalPt2, rightScoreTotalPt2);

    fclose(inputFile);


}
