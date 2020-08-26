/* errors.h contain the variable and function declaration of errors.c */
#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <string.h>
#include "global.h"


void handleError(fileData err);

void setErrorData(fileData *err, char * msg);

void allocateFailed();

#endif