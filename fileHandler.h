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

/* Instruction Counter */
int IC;

/* Data Counter */
int DC;

/* handle the given file, parse him and make an output files */
void readFile(char*);

/* checks the file line by line and making the basic machine code and symbol-table */
int firstPass(FILE *fp, fileData * fd);

/* completes the missing code and tag the extern symbols */
int secondPass(FILE *fp, fileData * fd);

/* insert all the missing info code(direct address, externs etc..) using the fix-list that contain all the cells that missing info */
void insertMissing(fileData * fd);

/* updates the code from given address using symbol-table */
void updateOp(fixNode * fixNode, fileData * fd);

/* create object file that contain all the addresses and their hexadecimal value */
void createObject(char *fName);

/* creates entry file that contain all the entries symbols and their addresses */
void createEntries(char *fName);

/* create extern file that contain all the extern symbols and their addresses */
void createExtern(char *fName);

/* create object, entry, external files by given name */
void createFiles(char *fName);

/* free all the allocated memory in the program */
void freeAll(void);

/* free all the allocated memory in the symbol list */
void freeSymbolList(void);

/* free all the allocated memory in the data list */
void freeDataList(void);

/* free all the allocated memory in the instruction list */
void freeCmdList(void);

/* free all the allocated memory in the external list */
void freeExternList(void);

/* free all the allocated memory in the fixe list */
void freeFixList(void);

/* gets the tail of the data list */
dataNode * getDataTail(void);

/* gets the tail of the symbol list */
symbolNode * getSymbolTail(void);

/* gets the tail of the instruction list */
cmdNode * getCmdTail(void);

/* gets the tail of the external list */
externNode * getExtTail(void);

#endif