/* instruction.h contain all the instruction.c definitions */
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string.h>
#include <ctype.h>
#include "dataStructs.h"
#include "parse.h"

#define CMD_LEN 5
#define NUM_OF_CMD 17
#define NUM_OF_REG 8

/*functions prototype */
void cmdParse(char*,char*, fileData * fd);
int checkCmd(char*);
int noParCheck(char * line, fileData * fd);
void twoParsCheck(char*,int,int*, fileData * fd);
void oneParCheck(char*,int,int*, fileData * fd);
int immediateAddress(char*, fileData * fd);
int directAddress(char*, fileData * fd);
int relativeAddress(char*, fileData * fd);
int registerAddress(char*, fileData * fd);
void * setWordNoPars(int *word, int type);

/**
 * check if is valid char for some of commands
 * @param c char to cher
 * @return true if valid otherwise false
 */
int isValidSpecialCharsBase(char c, fileData * fd);
int getParam(char *params, char * save, fileData * fd);

/* two pars */
void Mov_Add_Sub_Pars(char*,char*, int*, fileData * fd);
void Cmp_Pars(char*,char*,int*, fileData * fd);
void Lea_Pars(char*,char*,int*, fileData * fd);
void setWordTwoPars(int *word, int type);

/* one par */
void Clr_Not_Inc_Dec_Red_Par(char*,int*, fileData * fd);
void Jmp_Bne_Jsr_Par(char*,int*, fileData * fd);
void Prn_Pars(char*,int*, fileData * fd);
void setWordOnePar(int *word, int type);

void handleAddressingTwoParams(int, char *, char *, int *, fileData * fd);
void handleAddressingOneParam(int, char *, int *, fileData * fd);
void handleRegister(int *word, int *p, char *par, int target);
void handleImmidiate(int *word, int *wordPar, int *flag, int *p, char *par);
void handleDirect(int *word, int *wordPar, int isSource);

#endif
