/* symbolHandler.h contain all the symbolHandler.c defenition and macros */
#include <string.h>
#include <ctype.h>
/*#include <stdlib.h>*/
#include "dataStructs.h"

#include "errors.h"

#define FIRST_PASS 1
#define SECOND_PASS 2

/* updated */

#define BLANKJMP(x) while(isspace(*x)) (x)++; /* jump above blanks */



extern int IC;
extern int DC;

#ifndef STRUCTURES
#define STRUCTURES

typedef enum{SYMBOL_CODE = 1, SYMBOL_DATA = 2, SYMBOL_ENTRY = 4, SYMBOL_EXTERNAL = 8}symbolType;
typedef struct symbolNode /* symbol table node */
{
    char name[SYMBOL_LEN]; /* name of symbol */
    unsigned long int value; /* address of symbol */
    char type; /* the type of the symbol */
    struct symbolNode *next; /* next node */
    struct symbolNode *prev; /* prev node */
}symbolNode;

typedef struct externNode /* extern list node */
{
    char sym[SYMBOL_LEN]; /* symbol name */
    unsigned long int address; /* the address to save */
    struct externNode *next; /* next node */
    struct externNode *prev; /* prev node */
}externNode;

externNode *extHead, *extTail;
symbolNode *symHead, *symTail;
#endif

/* function prototype */
void checkSymbol(char*,int, fileData * fd);
void syntaxCheck(char*, fileData * fd);
void existence(char*,int, fileData * fd);
void updateStable(void);
void insertSymbol(char*, int);
void addSymbol(char*, int);
void creatTable(char*, int);
void entryMark(char*);
void insertExt(char*, unsigned long int);
void createExt(char*, unsigned long int);
void addExt(char*, unsigned long int);
void resetSym(symbolNode *node);