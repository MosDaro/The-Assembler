/**
 * Authors: Moisei Shkil & Shimon Biton
 * Date: 29/08/20
 * Program Description: The program assembler gets from the user files' names without a suffix in the command line that contains assembly code.
 *  The assembler runs on each file and checks the syntax. 
 *  Afterwards it translates each line from the given files to Hexadecimal code which is the Machine code of each line.
 *  When the file is translated successfully the assembler creates 3 types of out files: 
 *   * <input file name>.ob = file that contains all the addresses and the Machine code value of each line from the given file.
 *   * <input file name>.ent = file that contains all the entry symbols in the given input file and their addresses.
 *   * <input file name>.ext = file that contains all the external symbols in the given input file and their addresses.
 *  The assembler uses 2 passes:
 *   * In the first pass, the assembler creates a symbol table and a structure of data and code memory picture and maintains it during the run.
 *   * In the second pass, the assembler fills the missing values to complete the memory picture, and then creates output files.
 * 
 * -----------------------
 * File Name: assemlber.c
 * File Description: This file is contains the main function and start the translation of the each file by loop.
 * 
 */

#include "fileHandler.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START DELETE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*@todo delete before finish*/
#define DEBUG 0
void test();
void checkOriginalInput();
void checkErrorsInput();
void checkTest2ValidInput();
int checkValidateCode(char * resFile, char * fileResCmp);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END DELETE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * Function: main
 * Function Description: loop the given files and start the checks and the translation of them.
 * 
 * @param argc arguments count
 * @param argv arguments values
 * @return program exit staus
 */
int main(int argc, char *argv[]){
    int i, filesToCompile = false;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START DELETE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    if(DEBUG) {
        test();
        return 1;
    }
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END DELETE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */    

    if(argc == 1){ /* the argument is only the program name */
        fprintf(stderr,"No files to translate.\n");
    }
    else{
        filesToCompile = true;
    }
        
    if(filesToCompile){
        for(i = 1; i < argc; i++){ /* loop through file names */
            readFile(argv[i]); /* start translation of the current file */
        }
    }
    return 0;
} /* end of main function */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START DELETE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void test(){
    printf("*****************************\n");
    printf("test mode\n");
    printf("*****************************\n");
    printf("\n\n\n");

    checkErrorsInput();
    checkOriginalInput();
    checkTest2ValidInput();
}

void checkOriginalInput(){
    char srcFile[] = "tests/test1";
    char resFile[] = "tests/test1.ob";
    char fileResCmp[] = "tests/test1_p.txt";
    int isValid;

    readFile(srcFile);

    isValid = checkValidateCode(resFile, fileResCmp);
    if(isValid){
        printf("Congratulations file %s is validate\n", srcFile);
    }
}

void checkTest2ValidInput(){
    char srcFile[] = "tests/test_2";
    char resFile[] = "tests/test_2.ob";
    char fileResCmp[] = "tests/test_2_p.txt";
    int isValid;

    readFile(srcFile);

    isValid = checkValidateCode(resFile, fileResCmp);
    if(isValid){
        printf("Congratulations file %s is validate\n", srcFile);
    }
}

void checkErrorsInput(){
    char srcFile[] = "tests/test_k";
    char srcFileRes[] = "tests/temp.txt";
    char fileResCmp[] = "tests/test_p.txt";
    int isValid;
    FILE * f;
    FILE * handle;

    f = fopen(srcFileRes, "w");
    if(!f){
        printf("Can't open file %s\n", srcFileRes);
        return;
    }
    handle = stderr;
    stderr = f;

    readFile(srcFile);
    fclose(f);
    stderr = handle;

    isValid = checkValidateCode(srcFileRes, fileResCmp);
    if(isValid){
        printf("Congratulations file %s is validate\n", srcFile);
    }
}


int checkValidateCode(char * resFile, char * fileResCmp){
    FILE * src;
    FILE * dst;
    char lineSrc[1000], lineDst[1000];
    char * res1, * res2;
    int lineCounter = 1, isValid = true;

    src = fopen(resFile, "r");
    dst = fopen(fileResCmp, "r");
    while (true){
        res1 = fgets(lineSrc, 1000, src);
        res2 = fgets(lineDst, 1000, dst);
        if(!res1 || !res2){
            if((res1 && !res2) || (res2 && !res1)){
                isValid = false;
                fprintf(stderr, "one file is continue but other is finish but not equal!!!!");
            }
            break;
        }
        if(strcmp(lineSrc, lineDst)) {
            isValid = false;
            fprintf(stderr,"In file %s at line %d not compare to %s\n", resFile, lineCounter, fileResCmp);
        }
        lineCounter++;
    }

    fclose(src);
    fclose(dst);
    return isValid;
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END DELETE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */