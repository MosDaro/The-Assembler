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

/* parse the instruction and their parameters */
void cmdParse(char*,char*, fileData * fd);

/* search instruction number from cmdList and return the number, if not found return 0 */
int checkCmd(char*);

/* checks if there no parameters */
int noParCheck(char * line, fileData * fd);

/* handles instruction with two parameters and checks if the parameters is valid */
void twoParsCheck(char*,int,int*, fileData * fd);

/* handle instruction with one parameter and checks if the parameter is valid */
void oneParCheck(char*,int,int*, fileData * fd);

/* immediate addressing check (method number 0) */
int immediateAddress(char*, fileData * fd);

/* direct addressing check (method number 1) */
int directAddress(char*, fileData * fd);

/* direct relative addressing check (method number 2) */
int relativeAddress(char*, fileData * fd);

/* register addressing check (method number 3) */
int registerAddress(char*, fileData * fd);

/* set the word to funct with no parameters */
void * setWordNoPars(int *word, int type);

/* checks if the given char is valid */
int isValidSpecialCharsBase(char c, fileData * fd);

/* */
int getParam(char *params, char * save, fileData * fd);

/* hanndle the parameters of instructions mov, add and sub, decide which param is what type and set the word */
void Mov_Add_Sub_Pars(char*,char*, int*, fileData * fd);

/* hanndle the parameters of instruction cmp, decide which param is what type and set the word */
void Cmp_Pars(char*,char*,int*, fileData * fd);

/* handle the parameters of instruction lea, decide which param is what type and set the word */
void Lea_Pars(char*,char*,int*, fileData * fd);

/* sets the word to compatible funct */
void setWordTwoPars(int *word, int type);

/* hanndle the parameters of instructions clr, not, inc, dec, red and pardecide which param is what type and set the word */
void Clr_Not_Inc_Dec_Red_Par(char*,int*, fileData * fd);

/* handle the parameter of instructions jmp,bne,jsr decide which param is what type and set the word */
void Jmp_Bne_Jsr_Par(char*,int*, fileData * fd);

/* hanndle the parameters of instruction prn, decide which param is what type and set the word */
void Prn_Pars(char*,int*, fileData * fd);

/* sets the word to compatible funct */
void setWordOnePar(int *word, int type);

/* handles the groups that have the same addresing in the same parameters */
void handleAddressingTwoParams(int, char *, char *, int *, fileData * fd);

/* handles the groups that have the same addresing parameter */
void handleAddressingOneParam(int, char *, int *, fileData * fd);

/* sets the word to compatible register */
void handleRegister(int *word, int *p, char *par, int target);

/* sets the word to immidiate parameter */
void handleImmidiate(int *word, int *wordPar, int *flag, int *p, char *par);

/* sets the word to directive parameter */
void handleDirect(int *word, int *wordPar, int isSource);

#endif
