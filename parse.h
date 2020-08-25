/* parse.h contain all the parse.c definitions */

#ifndef PARCE_H
#define PARCE_H

#include "symbolHandler.h"
#include "errors.h"
#include "dataStructs.h"
#include "instruction.h"

#define LINE_LEN 81
#define DIRECTIVE_LEN 7
#define LINE_OVER_MAX_SIZE 100

extern int DC;
extern int IC;


void lineParse(char*, int, fileData * fd);
void dirParse(char*, int, fileData * fd);
void dataCheck(char*, fileData * fd);
void stringCheck(char*, fileData * fd);
void getSymbolParam(char * line, char * save, unsigned int * nextIndex, fileData * fd);
int checkDirective(char*, fileData * fd);
int dataValidation(char *str);
void dataCommaValidation(int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd);
void dataSignValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd);
void dataDigitValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd);

#endif