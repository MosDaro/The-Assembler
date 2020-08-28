/* errors.h contain the variable and function declaration of errors.c */
#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <string.h>
#include "global.h"

/* Prints the given error */
void handleError(fileData err);

/* Prepares the error for printing */
void setErrorData(fileData *err, char * msg);

/* This function handles the exception of allocation failure */
void allocateFailed();

#endif