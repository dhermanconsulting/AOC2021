#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_LENGTH 255
#define MAX_NAME_LENGTH 50
#define MAX_SUB_DIRECTORIES 255
#define MAX_SUB_FILES 255
#define MAX_DIRECTORIES_TOTAL 1024


struct File {

    char FileName[MAX_NAME_LENGTH];
    int FileSize;

};

struct Directory {

    struct Directory *ParentDirectory; // Pointer to the parent directory

    char DirectoryName[MAX_NAME_LENGTH]; // Name of this Directory

    struct Directory *ChildDirectories[MAX_SUB_DIRECTORIES]; // Array of pointers to sub directories
    int ChildDirectoryCount;

    struct File *Files [MAX_SUB_FILES]; //  Array of pointers to sub directories
    int FilesCount;

    int DirectorySize;

};

void GetDirectorySizeList(struct Directory *ParentDirectory, int *ResultArray, int *index){

    int curIndex = *index;

    ResultArray[curIndex] = ParentDirectory->DirectorySize;
    curIndex++;

    *index = curIndex;

    for(int i = 1; i <= ParentDirectory->ChildDirectoryCount; i++) {

        struct Directory *directory = ParentDirectory->ChildDirectories[(i-1)];

        GetDirectorySizeList(directory, ResultArray, index);

    }


}

int CalculateDirectorySize(struct Directory *ParentDirectory) {

    int directoryTotal = 0;

    for(int i = 1; i <= ParentDirectory->FilesCount; i++) {

        struct File file = *ParentDirectory->Files[(i-1)];

        directoryTotal += file.FileSize;

    }


    for(int i = 1; i <= ParentDirectory->ChildDirectoryCount; i++) {

        struct Directory *directory = ParentDirectory->ChildDirectories[(i-1)];

        int subDirSize = CalculateDirectorySize(directory);

        directoryTotal+=subDirSize;
    }


    ParentDirectory->DirectorySize = directoryTotal;
    return directoryTotal;

}

void PrintFileSystem(struct Directory *ParentDirectory, int Depth) {


    char indent[100] = {0};

    int indentCount = Depth * 5;

    for(int i = 0; i < indentCount; i++){

        indent[i] = ' ';

    }

    printf("%s<DIR> %s\n", indent, ParentDirectory->DirectoryName);

    indentCount += 5;
    for(int i = 0; i < indentCount; i++){

        indent[i] = ' ';

    }

    for(int i = 1; i <= ParentDirectory->FilesCount; i++) {


        struct File file = *ParentDirectory->Files[(i-1)];
        printf("%s%s (%i)\n",indent, file.FileName, file.FileSize);

    }

    for(int i = 1; i <= ParentDirectory->ChildDirectoryCount; i++) {

        struct Directory directory = *ParentDirectory->ChildDirectories[(i-1)];
        PrintFileSystem(&directory, Depth++);

    }

}

void AddDirectory(struct Directory *ParentDirectory, char Name[MAX_NAME_LENGTH]) {

    // Create new directory
    struct Directory *tmpDirectory = calloc(1, sizeof(struct Directory));
    tmpDirectory->ParentDirectory = ParentDirectory;
    strcpy(&(tmpDirectory->DirectoryName), Name);

    // We now have a new directory sat in memory, so reference it

    ParentDirectory->ChildDirectories[ParentDirectory->ChildDirectoryCount] = tmpDirectory;
    ParentDirectory->ChildDirectoryCount++;
}

void AddFile(struct Directory *ParentDirectory, char Name[MAX_NAME_LENGTH], int FileSize) {

    // Create new tmpFile
    struct File *tmpFile = calloc(1,sizeof(struct Directory));
    tmpFile->FileSize = FileSize;
    strcpy(&(tmpFile->FileName), Name);

    // We now have a new tmpFile sat in memory, so reference it
    ParentDirectory->Files[ParentDirectory->FilesCount] = tmpFile;
    ParentDirectory->FilesCount++;
}

void ChangeDirectory(struct Directory **CurrentDirectory, char Target[MAX_NAME_LENGTH]) {

    for(int i = 0; i < (*CurrentDirectory)->ChildDirectoryCount; i++) {

        struct Directory *nextDir = (*CurrentDirectory)->ChildDirectories[i];

        int strCom = strncmp(nextDir->DirectoryName, Target, MAX_NAME_LENGTH);

        if(strCom == 0) {
            *CurrentDirectory = nextDir;
            return;
        }

    }
    printf("Didn't find a DIR!");

}

void DoInstruction(struct Directory **CurrentDirectory, char *Buffer, struct Directory *RootDirectory) {

    int isListInstruction = strncmp(Buffer, &"$ ls", 4);
    int isCdInstruction =  strncmp(Buffer, &"$ cd", 4);

    if(isListInstruction == 0) {
        // At the minute I don't think we actually have to do anything
        return;
    }

    if(isCdInstruction == 0) {

        char cdTarget = Buffer[5];

        switch(cdTarget) {
            case '.':
                // Move up a level
                *CurrentDirectory = (*CurrentDirectory)->ParentDirectory;
                return;
            case '/':
                // Go back to root
                *CurrentDirectory = RootDirectory;
                return;

        }

        char directoryName[MAX_NAME_LENGTH] = {0};

        char *nameStart = &Buffer[5];

        strcpy(directoryName, nameStart);

        directoryName[strcspn(directoryName, "\n")] = 0; // Remove trailing newline

        ChangeDirectory(CurrentDirectory,directoryName );
    }

}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Missing input\n");
        return -1;
    }

    struct Directory root = {0};
    strcpy(root.DirectoryName, "root");
    struct Directory *currentDirectory = NULL;

    struct Directory *allDirectories = calloc(MAX_DIRECTORIES_TOTAL, sizeof(struct Directory));

    // Open the file for reading
    char buffer[BUFFER_LENGTH] = {0};
    FILE *inputFile = fopen(argv[1], "r");

    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {

        if(buffer[0] == '$') {

            // This is an instruction
            DoInstruction(&currentDirectory, &buffer, &root);
            continue;
        }

        int isDir = strncmp(buffer, &"dir", 3);

        if(isDir == 0) {
            // Describes a DIR, so create it and add it to the current dir
            char directoryName[MAX_NAME_LENGTH] = {0};

            char *nameStart = &buffer[4];

            strcpy(directoryName, nameStart);

            directoryName[strcspn(directoryName, "\n")] = 0; // Remove trailing newline


            AddDirectory(currentDirectory, directoryName);
            continue;
        }

        int isFile = isdigit(buffer[0]);

        if (isFile == 1) {
            // Line describes a file

            char *endFileSize = NULL;

            int fileSize = strtol(buffer, &endFileSize, 0);

            char fileName[MAX_NAME_LENGTH];
            strcpy(fileName, (endFileSize + sizeof(char))); // Add a character to the pointer, to trim the space
            fileName[strcspn(fileName, "\n")] = 0; // Remove trailing newline

            AddFile(currentDirectory, fileName, fileSize);

            continue;
        }

        return 99; // Shouldn't ever end up here
    }


    //PrintFileSystem(&root, 0);

    CalculateDirectorySize(&root);

    int sizeIndex[1024] = {0};

    int index = 0;
    GetDirectorySizeList(&root, &sizeIndex, &index);

    int countAbove = 0;

    for(int i = 0; i < 1024; i++) {

        if(sizeIndex[i] <= 100000 ) {
            countAbove+=sizeIndex[i];
        }

    }

    // Part 2

    int totalInUse = root.DirectorySize;

    int freeSpace = 70000000 - totalInUse;
    int requiredSpace = 30000000 - freeSpace;

    int smallestContender = 0;

    for(int i = 0; i < 1024; i++) {

        if(sizeIndex[i] >= requiredSpace) {

            if(smallestContender == 0) {
                smallestContender = sizeIndex[i];
                continue;
            }

            if(sizeIndex[i] <= smallestContender) {
                smallestContender = sizeIndex[i];
            }

        }

    }

    printf("Part 1: %i", countAbove);
    printf("Part 2: %i", smallestContender);
    return 0;
}
