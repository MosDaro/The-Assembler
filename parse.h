/* parse.h contain all the parse.c definitions */

#ifndef PARCE_H
#define PARCE_H

#include <limits.h>
#include "symbolHandler.h"

#define LINE_LEN 81
#define DIRECTIVE_LEN 7
#define LINE_OVER_MAX_SIZE 100
#define Dozen 10

/* Data Counter */
extern int DC;

/* Instrucation Counter */
extern int IC;

/**
 * parse the given line and making symbol-table and the basic code("word") value 
 * handle two passed the first make the basic code and label-table the second handle the externals
 */
void lineParse(char*, int, fileData * fd);

/* Parse the directive and their parameters */
void dirParse(char*, int, fileData * fd);

/* Checks the data parameters using flag for each type of valid char */
void dataCheck(char*, fileData * fd);

/* Check the string directive parameters and insert to data list */
void stringCheck(char*, fileData * fd);

/* Checks and insert symbols' params */
void getSymbolParam(char * line, char * save, unsigned int * nextIndex, fileData * fd);

/* Checks the directive and return the number of directive */
int checkDirective(char*, fileData * fd);

/* Checks if the string contain only valid chars */
int dataValidation(char *str);

/* Checks the validity of the the comma in data parameters and turns on the relevant */
void dataCommaValidation(int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd);

/* Checks the validity of the negetive/positive sign in data parameters and turns on the relevant */
void dataSignValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd);

/* Checks the validity of the digit in data parameters and turns on the relevant */
void dataDigitValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd);

/* converts string to int */
int alternativeAtoi(char* str);

#endif