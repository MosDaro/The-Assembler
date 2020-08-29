#ifndef ASSEMBLER_GLOBAL_H
#define ASSEMBLER_GLOBAL_H

#include <stdio.h>
#include <string.h>

#define FILE_NAME_LEN 30
#define MSG_LEN 100

/* struct to follow file name, line number and errors */
typedef struct {
    char fileName[FILE_NAME_LEN];
    int lineNumber;
    char msg[MSG_LEN];
    int isHasError;
} fileData;


/* Prints the given error */
void handleError(fileData err);

/* Prepares the error for printing */
void setErrorData(fileData *err, char * msg);

/* This function handles the exception of allocation failure */
void allocateFailed();

#endif