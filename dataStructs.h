/* dataStructs.h contain all the structs definitions of dataStructs.c and the another definitions */
#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <string.h>
#include "global.h"

#define FIELDS 3
#define MOVE_TO_FIRST 5
#define ONE_FIELD 8
#define NUM_BITS 24
#define FIRST_FIELD 255
#define SECOND_FIELD 65280
#define THIRD_FIELD 16711680
#define SYMBOL_LEN 32 /* max len of symbol */

/* data symbol type enum */
typedef enum{NOT_FOUND, DATA, STRING, ENTRY, EXTERN, CODE} directiveType;

/* all the instruction names */
enum cmdType{CMD_NOT_FOUND,MOV,CMP,ADD,SUB,LEA,CLR,NOT,INC,DEC,JMP,BNE,JSR,RED,PRN,RTS,STOP};

/* opcode bits in word */
enum opcodeBits{FIRST_BIT_OPCODE=18, SECOND_BIT_OPCODE, THIRD_BIT_OPCODE,FORTH_BIT_OPCODE, FIFTH_BIT_OPCODE, 
                 SIXTH_BIT_OPCODE};

/* registers bits in word */
enum registersBits{FIRST_REG_DEST=8, SECOND_REG_DEST, THIRD_REG_DEST,
                    FIRST_REG_SOUR=13, SECOND_REG_SOUR, THIRD_REG_SOUR};
    
/* addresing types bits in word */
enum addressingBits{FIRST_ADR_DEST=11, SECOND_ADR_DEST, FIRST_ADR_SOUR=16, SECOND_ADR_SOUR};

/* funct bits in word */
enum funct{FIRST_FNCT=3, SECOND_FNCT, THIRD_FNCT, FORTH_FNCT, FIFTH_FNCT};

/* ARE bits in word */
enum ARE{E_BIT,R_BIT,A_BIT};

/* list of data (all the data of .string and .data directive) */
typedef struct dataNode {
    unsigned char *val; /* bitfiled of 24 bits */
    struct dataNode *next; /* next node pointer */
    struct dataNode *prev; /* prev node pointer */
}dataNode;

/* the list of all the instructions values */
typedef struct cmdNode {
    unsigned char *val; /* bitfiled of 24 bits */
    struct cmdNode *next; /* next node pointer */
    struct cmdNode *prev; /* prev node pointer */
}cmdNode;

/* the fix list is all the unknown symbols list */
typedef struct fixNode {
    unsigned long int address; /* address of the symbol in instruction list */
    char symbol[SYMBOL_LEN]; /* the name of symbol */
    int line; /* number of line */
    int nodesCount; /* nodesCounter */
    struct fixNode *next; /* next node pointer */
    struct fixNode *prev; /* prev node pointer */
}fixNode;

/* Data Counter */
extern int DC;

/* Instruction Counter */
extern int IC;

/* head of linked list of the data values */
dataNode *dataHead;

/* head of linked list of unknown symbols */
fixNode *fixHead;

/* head of linked list of the instructions values */
cmdNode *cmdHead;

/* insert the given value to data list */
void insertData(unsigned long int value);

/* creates data list and insert the given value to the list */
void createData(unsigned long int value);

/* insert the given value to data list */
void addData(unsigned long int value);

/* insert the given value to instruction list and if the value unknown to fix list */
void insertCmd(unsigned long int value, unsigned long int address, char*, fileData * fd);

/* creates fix list and insert the given values to the head */
void createFix(unsigned long int adrs, char *sym, fileData * fd);

/* inserts the given address and symbol to fix list */
void addFix(unsigned long int adrs, char *sym, fileData * fd);

/* returns mask with given bit number on */
int makeMask(int bitNum);

/* creates list of instructions code and set the value */
void createCmd(unsigned long int value);

/* adds new node to cmd list and insert the value */
void addCMD(unsigned long int value);

/* inserts the given int value to the given pointer 24 bits field */
void setVal(unsigned char* word, unsigned long int value);

/* gets pointer of word and insert the value to int variable and return the result of word inserted to int */
int setInt(unsigned char* word);

/* moves all the bits 3 spots left to make space for ARE */
void clearARE(unsigned char *word);

/* resetAlocValFunction Description: reset all the 24 bits of given pointer */
void resetAlocVal(unsigned char *val);

#endif
  