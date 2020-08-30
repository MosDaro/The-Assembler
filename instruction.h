/* instruction.h contain all the instruction.c definitions */
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string.h>
#include <ctype.h>
#include "dataStructs.h"
#include "parse.h"

#define CMD_LEN 5 /* max len of command len */
#define NUM_OF_CMD 17 /* count of commands this assembly know (first is for unknown) */
#define NUM_OF_REG 8 /* count of registers use in this assembly */
#define NUM_OF_DIRECTIVE 4 /* count of registers use in this assembly */

/* parse the instruction and their parameters */
void cmdParse(char *sym, char *pars, fileData * fd);

/* search instruction number from cmdList and return the number, if not found return 0 */
int checkCmd(char *cmd);

/* checks if there no parameters and return if there is an error */
int noParCheck(char * line, fileData * fd);

/* handles instruction with two parameters and checks if the parameters is valid */
void twoParsCheck(char *pars, int type, int *word, fileData * fd);

/* handle instruction with one parameter and checks if the parameter is valid */
void oneParCheck(char *line, int type, int *word, fileData * fd);

/* immediate addressing check (method number 0) and return if the parameter is valid or not */
int immediateAddress(char *par, fileData * fd);

/* direct addressing check (method number 1) and return if the parameter is valid or not */
int directAddress(char *par, fileData * fd);

/* direct relative addressing check (method number 2) and return if the parameter is valid or not */
int relativeAddress(char *par, fileData * fd);

/* register addressing check (method number 3) and return if the parameter is valid or not */
int registerAddress(char *par , fileData * fd);

/* set the word to funct with no parameters */
void * setWordNoPars(int *word, int type);

/* checks if the given char is valid and return if valid or not */
int isValidSpecialCharsBase(char c, fileData * fd);

/* get string and copy param to save string if param is valid and return number of characters already read */
int getParam(char *params, char * save, fileData * fd);

/* handle the parameters of instructions mov, add and sub, decide which param is what type and set the word */
void Mov_Add_Sub_Pars(char *par1, char *par2, int *word, fileData * fd);

/* handle the parameters of instruction cmp, decide which param is what type and set the word */
void cmpPars(char *par1, char *par2, int *word, fileData * fd);

/* handle the parameters of instruction lea, decide which param is what type and set the word */
void leaPars(char *par1, char *par2, int *word, fileData * fd);

/* sets the word to compatible funct */
void setWordTwoPars(int *word, int type);

/* handle the parameters of instructions clr, not, inc, dec, red and pardecide which param is what type and set the word */
void Clr_Not_Inc_Dec_Red_Par(char *par, int *word, fileData * fd);

/* handle the parameter of instructions jmp,bne,jsr decide which param is what type and set the word */
void Jmp_Bne_Jsr_Par(char *par, int *word, fileData * fd);

/* handle the parameters of instruction prn, decide which param is what type and set the word */
void prnPars(char *par, int *word, fileData * fd);

/* sets the word to compatible funct */
void setWordOnePar(int *word, int type);

/* handles the groups that have the same addressing in the same parameters */
void handleAddressingTwoParams(int type, char *par1, char *par2, int *word, fileData * fd);

/* handles the groups that have the same addressing parameter */
void handleAddressingOneParam(int type, char *par, int *word, fileData * fd);

/* sets the word to compatible register */
void handleRegister(int *word, int *p, char *par, int target);

/* sets the word to immediate parameter */
void handleImmediate(int *word, int *wordPar, int *flag, int *p, char *par);

/* sets the word to directive parameter */
void handleDirect(int *word, int *wordPar, int isSource);

#endif
