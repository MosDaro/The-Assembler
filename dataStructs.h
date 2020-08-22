/* dataStructs.h contain all the structs definitions of dataStructs.c and the another definitions */
#include <stdlib.h>
#include <string.h>
#include "errors.h"

#define FIELDS 3
#define MOVE_TO_FIRST 5
#define ONE_FIELD 8
#define NUM_BITS 24
#define FIRST_FIELD 255
#define SECOND_FIELD 65280
#define THIRD_FIELD 16711680
#define SYMBOL_LEN 32


#ifndef ENUMS
#define ENUMS

enum {false, true};

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

#endif

#ifndef THEDATA
#define THEDATA
typedef struct dataNode /* list of data (all the data of .string and .data directive) */
{
    unsigned char *val; /* bitfiled of 24 bits */
    struct dataNode *next; /* next node pointer */
    struct dataNode *prev; /* prev node pointer */
}dataNode;

typedef struct cmdNode /* the list of all the instructions values */
{
    unsigned char *val; /* bitfiled of 24 bits */
    struct cmdNode *next; /* next node pointer */
    struct cmdNode *prev; /* prev node pointer */
}cmdNode;

typedef struct fixNode /* the fix list is all the unknown symbols list */
{
    unsigned long int address; /* address of the symbol in instruction list */
    char symbol[SYMBOL_LEN]; /* the name of symbol */
    int line; /* number of line */
    struct fixNode *next; /* next node pointer */
    struct fixNode *prev; /* prev node pointer */
}fixNode;
#endif

extern int DC;
extern int IC;
int ind;

/* heads and tails of lists */
dataNode *dataHead;
dataNode *dataTail;
fixNode *fixHead;
fixNode *fixTail;
cmdNode *cmdHead;
cmdNode *cmdTail;

/* function prototype */
void insertData(int value);
void createData(int value);
void addData(int value);
void insertCmd(int value, unsigned long int, char*, fileData * fd);
void createFix(unsigned long int adrs, char *sym, fileData * fd);
void addFix(unsigned long int adrs, char *sym, fileData * fd);
int makeMask(int bitNum);
void createCmd(int value);
void addCMD(int value);
void setVal(unsigned char* word, int value);
int setInt(unsigned char* word);
void clearARE(unsigned char *word);