/* symbolHandler.h contain all the symbolHandler.c defined and macros */
#ifndef SYMBOL_H
#define SYMBOL_H

#include <string.h>
#include <ctype.h>
#include "instruction.h"

#define FIRST_PASS 1 /* flag to first pass */
#define SECOND_PASS 2 /* flag to second pass */
#define BLANKJMP(x) while(isspace(*x)) (x)++; /* jump above blanks */

/* Instruction Counter */
extern int IC;

/* Data Counter */
extern int DC;

/* symbols types enum */
typedef enum{SYMBOL_CODE = 1, SYMBOL_DATA = 2, SYMBOL_ENTRY = 4, SYMBOL_EXTERNAL = 8}symbolType;

/* symbol table node */
typedef struct symbolNode {
    char name[SYMBOL_LEN]; /* name of symbol */
    unsigned long int value; /* address of symbol */
    char type; /* the type of the symbol */
    struct symbolNode *next; /* next node */
    struct symbolNode *prev; /* prev node */
}symbolNode;

/* extern list node */
typedef struct externNode { 
    char sym[SYMBOL_LEN]; /* symbol name */
    unsigned long int address; /* the address to save */
    struct externNode *next; /* next node */
    struct externNode *prev; /* prev node */
}externNode;

/* external head linked list */
externNode *extHead;

/* symbol head linked list */
symbolNode *symHead;

/* check if the symbol valid */
void checkSymbol(char*,int, fileData * fd);

/* if the symbol length is valid return true else false */
int checkSymbolLen(char * symbol, fileData *fd);

/* checks if the syntax of the symbol valid */
void syntaxCheck(char*, fileData * fd);

/* checks if the symbol exist, for first pass its an error for second its valid */
void existence(char*,int, fileData * fd);

/* updates the symbol table after the first pass */
void updateStable(void);

/* inserts the give symbol name and the type if have one to symbol table */
void insertSymbol(char *symbol, int dir);

/* inserts the given symbol to symbol table */
void addSymbol(char *symbol, int dir);

/* create a symbol table and insert the given symbol */
void creatTable(char *symbol, int dir);

/* marks the entry symbols in symbol table */
void entryMark(char *sym);

/* inserts the given symbol to extern list for the extern file creation */
void insertExt(char* sym, unsigned long int adrs);

/* creates extern-list and insert the given symbol */
void createExt(char* symbol, unsigned long int adrs);

/* insert the given symbol to extern list */
void addExt(char* symbol, unsigned long int adrs);

/* reset the symbol nodes' values of linked list */
void resetSym(symbolNode *node);

/* Checks the existance of the given symbol in extern table */
int externExistance(char *symbol);

#endif