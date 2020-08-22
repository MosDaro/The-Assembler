/* updated */

/* errors.h contain the variable and function declaration of errors.c */
#ifndef ERROS_H
#define ERROS_H

#include <stdio.h>
#include <string.h>
#include "global.h"


void handleError(fileData err);

void setErrorData(fileData *err, char * msg);

void allocateFailed();

#endif