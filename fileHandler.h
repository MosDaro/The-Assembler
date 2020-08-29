/* fileHandler.h is contain all the fileHandler.c definitions, macros and etc.. */
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "parse.h"

#define IC_START 100

/* open the given file by given style and checks if opened */
#define FILE_OPEN(f,x,y) f = fopen(x,y);\
    if(f == NULL)\
    {\
        fprintf(stdout,"Failed to open file \"%s\"\n",x);\
        return;\
    }\

typedef enum{SYMBOLE_NODE, DATA_NODE, CMD_NODE, EXTERN_NODE, FIX_NODE, NUMBER_OF_LISTS} listsTypes;

int IC; /* instruction count */
int DC; /* data count */
void Checker(int list);
void readFile(char*);
int firstPass(FILE*, fileData * fd);
int secondPass(FILE*, fileData * fd);
void insertMissing(fileData * fd);
void updateOp(fixNode * fixNode, fileData * fd);
void createObject(char*);
void createEntries(char*);
void createExtern(char*);
void createFiles(char*);
void freeAll(void);
void freeSymbolList(void);
void freeDataList(void);
void freeCmdList(void);
void freeExternList(void);
void freeFixList(void);

dataNode * getDataTail();
symbolNode * getSymbolTail();
cmdNode * getCmdTail();
externNode * getExtTail();

#endif