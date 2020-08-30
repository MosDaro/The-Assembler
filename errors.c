/**
 * Authors: Moisei Shkil & Shimon Biton
 * Date: 29/08/20
 * File Name: dataStructs.c
 * File Description: handle the error if appears and the errors output.
 */

#include "global.h"
#include "fileHandler.h"


/**
 * Function: handleError
 * Function Description: prints the given error and change
 * @param err struct with description file handle now and error details
 */
void handleError(fileData err){
    fprintf(stderr,"Error found in file %s at line %d: %s\n", err.fileName, err.lineNumber, err.msg);
}

/**
 * Function: setErrorData
 * Function Description: prepares the error for printing and power on flag to know is has error
 * @param err struct with description file handle now and error details
 * @param msg message to print
 */
void setErrorData(fileData *err, char * msg){
    strcpy(err->msg, msg);
    err->isHasError = true;
    handleError(*err);
}

/**
 * Function: allocateFailed 
 * Function Description: handles the exception of allocation failure
 */
void allocateFailed(){
    freeAll(); /* free the allocated memory at program. */
    fprintf(stderr,"Memory allocation failed\n");
    exit(0);
}