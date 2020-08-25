#include "fileHandler.h"

/*@todo delete before finish*/
#define DEBUG 0
int test();
void checkOriginalInput();
void checkErrorsInput();
int checkValidateCode(char * resFile, char * fileResCmp);


int main(int argc, char *argv[]){
    int i;
    int filesToCompile = false;

    if(DEBUG) {
        test();
        return 1;
    }
    
    if(argc == 1) /* the argument only the program name */
        fprintf(stderr,"No files to translate.\n");
    else
        filesToCompile = true;
        
    if(filesToCompile)
        for(i = 1; i < argc; i++) /* runs on each file */
            readFile(argv[i]); /* deals with the current file */
    return 0;
}


int test(){
    printf("*****************************\n");
    printf("test mode\n");
    printf("*****************************\n");
    printf("\n\n\n");

    checkErrorsInput();
    checkOriginalInput();
}

void checkOriginalInput(){
    char srcFile[] = "../tests/test1";
    char resFile[] = "../tests/test1.ob";
    char fileResCmp[] = "../tests/test1_p.txt";
    int isValid;

    readFile(srcFile);

    isValid = checkValidateCode(resFile, fileResCmp);
    if(isValid){
        printf("Congratulations file %s is validate\n", srcFile);
    }
}

void checkErrorsInput(){
    char srcFile[] = "../tests/test_k";
    char srcFileRes[] = "../tests/temp.txt";
    char fileResCmp[] = "../tests/test_p.txt";
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
