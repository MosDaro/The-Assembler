/* errors.c handle the error if appears and the errors output */
#ifndef ASSEMBLER_GLOBAL_H
#define ASSEMBLER_GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* boolean enum */
enum {false, true};

#define FILE_NAME_LEN 30 /* max len of file name */
#define MSG_LEN 100 /* max len of error messages */

/* struct to follow file name, line number and errors */
typedef struct {
    char fileName[FILE_NAME_LEN];
    int lineNumber;
    char msg[MSG_LEN];
    int isHasError;
} fileData;


/* Prints the given error */
void handleError(fileData err);

/* Prepares the error details for printing and for sign error appears */
void setErrorData(fileData *err, char * msg);

/* This function handles the exception of allocation failure */
void allocateFailed();

#endif