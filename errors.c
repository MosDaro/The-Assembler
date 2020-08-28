/* errors.c handle the error count and the errors output */
#include <stdlib.h>
#include "errors.h"
#include "fileHandler.h"


/**
 * Function: handleError
 * 
 * prints the given error
 * 
 * @param err
 */
void handleError(fileData err){
    fprintf(stderr,"Error found in file %s at line %d: %s\n", err.fileName, err.lineNumber, err.msg);
}


/**
 * Function:  setErrorData
 * 
 * prepares the error for printing:
 * 
 * @param err
 * @param msg
 */
void setErrorData(fileData *err, char * msg){
    strcpy(err->msg, msg);
    err->isHasError = true;
    handleError(*err);
}

/**
 * Function:  allocateFailed 
 *
 * handles the exception of allocation failure:
 */
void allocateFailed(){
    freeAll(); /* free the allocated memory at program. */
    fprintf(stderr,"Memory allocation failed\n");
    exit(0);
}