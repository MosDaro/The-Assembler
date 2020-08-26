/* errors.c handle the error count and the errors output */
#include <stdlib.h>
#include "errors.h"
#include "fileHandler.h"


void handleError(fileData err){
    fprintf(stderr,"Error found in file %s at line %d: %s\n", err.fileName, err.lineNumber, err.msg);
}

void setErrorData(fileData *err, char * msg){
    strcpy(err->msg, msg);
    err->isHasError = 1;
    handleError(*err);
}

void allocateFailed(){
    freeAll();
    fprintf(stderr,"Memory allocation failed\n");
    exit(0);
}