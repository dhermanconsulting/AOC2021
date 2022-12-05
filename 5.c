#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACKS 10
#define MAX_STACK_HEIGHT 8
#define BUFFER_LENGTH 255

struct Crate {
    char Identifier;
    struct Crate *NextCrate;
};

struct Stack {
    int ID;
    struct Crate *TopCrate;
};


struct Instruction {
    int Quantity;
    int Source;
    int Destination;
};

struct Crate* PopCrate(struct Stack *stack) {

    if((*stack).TopCrate == NULL){
        printf("Warning, nothing to collect on stack");
    }

    // Get the current top crate
    struct Crate *topCrate = (*stack).TopCrate;

    // Get the next crate
    struct Crate *nextCrate = (*topCrate).NextCrate;

    // Replace the top crate with the second one
    (*stack).TopCrate = nextCrate;

    // Orphan the current top crate
    (*topCrate).NextCrate = NULL;

    // Return the top crate
    return topCrate;

}

void PushCrate(struct Stack *stack, struct Crate *crate) {

    // Get the current top crate from the stack
    struct Crate *topCrate = (*stack).TopCrate;

    // Replace the top crate with the new one
    (*stack).TopCrate = crate;

    // Point the new one to the old new one, completing the list
    (*crate).NextCrate = topCrate;

}



struct Instruction ParseInstruction(char *buffer){

    //char *startPtr = &buffer[5];



   // int num = strtol(&buffer[5], NULL, 0);


    // Get the length of the string
   // char charQty = buffer[5];

    char charSrc;
    char charDst;

   if(isdigit(buffer[6])) {
       charSrc = buffer[13];
       charDst = buffer[18];
   }else{
       charSrc = buffer[12];
       charDst = buffer[17];
   }


    struct Instruction instruction = {0};
    instruction.Quantity = strtol(&buffer[5], NULL, 0);
    instruction.Source = charSrc - '0';
    instruction.Destination = charDst - '0';

    return instruction;
}

void ExecuteInstruction(struct Instruction instruction, struct Stack stack[MAX_STACKS]){

    for(int i = 0; i < instruction.Quantity; i++){

        struct Crate *crate = PopCrate(&stack[instruction.Source]);
        PushCrate(&stack[0],crate);

    }

    for(int i = 0; i < instruction.Quantity; i++){

        struct Crate *crate = PopCrate(&stack[0]);
        PushCrate(&stack[instruction.Destination],crate);

    }



}

// Build up the stacks, based on the drawing

void ParseDiagram(struct Stack stackArray[MAX_STACKS], char diagram[MAX_STACK_HEIGHT][BUFFER_LENGTH]) {

   // printf(diagram[3]);

    // Init the stacks

    for(int stackIndex = 0; stackIndex < MAX_STACKS; stackIndex++){
        struct Stack stack;
        stack.TopCrate = NULL;
        stack.ID = stackIndex;
        stackArray[stackIndex] = stack;

    }

    // Work backwards, find the line with the definitions
    for(int line = MAX_STACK_HEIGHT; line >= 0; line--) {

        char *charArray = diagram;
        charArray + (line * (sizeof(char) * BUFFER_LENGTH));
        char st = diagram[line][0];

        if(diagram[line][0] != '[' && diagram[line][0] != ' ') continue; // Line isn't useful

        // Go through the line and numerate each stack
        for(int stackIndex = 1; stackIndex < MAX_STACKS; stackIndex++) {

            int charIndex = (stackIndex * 4) - 3; // Central character column for each stack
            char ident = diagram[line][charIndex];

            if(!isalpha(ident)) continue; // No crate here

            struct Crate *crate = calloc(1, sizeof(struct Crate));
            crate->Identifier = ident;
            crate->NextCrate = NULL;

            printf("Putting Crate %c on Stack %i\n", ident, stackIndex);

            PushCrate(&(stackArray[stackIndex]), crate);

        }

    }

}

void PrintTopOfStack(struct Stack stackArray[MAX_STACKS]) {

    for(int i = 0; i < MAX_STACKS; i++){

        char TopCrate = ' ';

        if(stackArray[i].TopCrate != NULL) {
              TopCrate = stackArray[i].TopCrate->Identifier;
        }

        printf("Stack %i : %c\n", i, TopCrate);


    }

}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Missing input\n");
        exit(-1);
    }

    // Open the file for reading
    FILE *inputFile = fopen(argv[1], "r");

    char diagram[MAX_STACK_HEIGHT][BUFFER_LENGTH] = {0};

    char buffer[BUFFER_LENGTH] = {0};
    int line = 0;

    // Assume the top portion of the file is the diagram and read it in
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        if(line < MAX_STACK_HEIGHT ) {
            memcpy((&diagram[line]),&buffer, sizeof(buffer));
            line++;
            continue;
        }

    }

    rewind(inputFile);

    struct Stack stackArray[MAX_STACKS];
    ParseDiagram(stackArray, diagram);

    PrintTopOfStack(stackArray);


    int currentLine = 0;
    // Now we go
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        printf("Process line %i", currentLine);

        if(buffer[0] != 'm') continue; // Not an instruction line

        struct Instruction instruction = ParseInstruction(buffer);

        ExecuteInstruction(instruction, stackArray);

        currentLine++;
    }

    PrintTopOfStack(stackArray);
    return 0;
}
