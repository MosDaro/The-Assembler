/* updated */


#include "fileHandler.h"

int main(int argc, char *argv[]){
    int i;
    int filesToCompile = false;
    
    if(argc == 1) /* the argument only the program name */
        fprintf(stderr,"No files to translate.\n");
    else
        filesToCompile = true;
        
    if(filesToCompile)
        for(i = 1; i < argc; i++) /* runs on each file */
            readFile(argv[i]); /* deals with the current file */
    return 0;
}