/**
 * Authors: Moisei Shkil & Shimon Biton
 * Date: 29/08/20
 * File Name: parse.c
 * File Description: contain all the function that parse info from file.
 */
#include "parse.h"

/**
 * Function Name: getSymbolParam
 * Function Description: saves the parameter of symbol
 * @param line pointer to string with line to parse
 * @param save pointer to string to save param if valid else empty string
 * @param nextIndex index start to check in line
 * @param fd file information
 */
void getSymbolParam(char * line, char * save, unsigned int * nextIndex, fileData * fd){
    char copy[LINE_OVER_MAX_SIZE], *temp, *c;
    unsigned int index = 0;

    save[0] = '\0';
    strcpy(copy, line);
    c = copy;

    BLANKJMP(c) /* ignore blanks */
    index = c - copy;
    *nextIndex += index;

    if(!strlen(c)){ /* no parameters */
        setErrorData(fd, "Missing symbol parameters");
    }
    if(!fd->isHasError && !isalpha(*c)){ /* the symbol is invalid */
        setErrorData(fd, "Symbol must started with letter");
    }
    if(!fd->isHasError) {
        temp = strtok(c, " ");
        strcpy(save, temp);
        *nextIndex += strlen(temp);

        if (strlen(copy + index) != strlen(temp)) { /* check if has letter after symbol parameter */
            c += strlen(temp);
            BLANKJMP(c) /* ignore blanks */
            if (*c && *c != '\n') {
                setErrorData(fd, "Extra letter after symbol");
            }
        }
    }
}

/**
 * Function Name: 
 * Function Description:
 * @param str pointer to string with line to parse
 * @param save pointer to string to save param if valid else empty string
 * @param nextIndex index start to check in line
 * @param fd file information
 */
void getSymbol(char * str, char * save, unsigned int * nextIndex, fileData * fd){
    char * temp, copy[LINE_OVER_MAX_SIZE];

    strcpy(copy, str);
    temp = copy;
    BLANKJMP(temp) /* ignore blanks */
    *nextIndex = temp - copy;
    temp = strtok(temp, ":");
    if (strlen(str + *nextIndex) == strlen(temp)) { /* not found : isn't have label */
        save[0] = '\0';
    }else {
        if(!checkSymbolLen(temp, fd)){
            save[0] = '\0';
        } else {
            strcpy(save, temp);
            *nextIndex += strlen(temp);
            (*nextIndex)++; /* ignore : */
            checkSymbol(save, FIRST_PASS, fd); /* check the symbol */
        }
    }
}

/**
 * Function Name: 
 * Function Description:
 * @param line pointer to string with line to parse
 * @param save pointer to string to save param if valid else empty string
 * @param nextIndex index start to check in line
 * @param fd file information
 */
void getDirective(char * line, char * save, unsigned int * nextIndex, fileData * fd){
    char * temp, copy[LINE_OVER_MAX_SIZE];

    save[0] = '\0';
    strcpy(copy, &line[*nextIndex]);
    temp = copy;
    BLANKJMP(temp);
    *nextIndex += temp - copy;

    if(*temp == '.'){ /* has directive */
        (*nextIndex)++; /* ignore . */
        temp++;
        temp = strtok(temp, " ");
        if(strlen(line + *nextIndex) == strlen(temp)){ /* not found word */
            setErrorData(fd, "Directive without word");
        }else{
            strcpy(save, temp);
            *nextIndex += strlen(temp);
        }
    }
}

/**
 * Function Name: 
 * Function Description:
 * @param line pointer to string with line to parse
 * @param fd file information
 * @return true if need to parse this line
 */
int isNeedCheckLine(char * line, fileData * fd){
    int isValid = true;
    char *lineCopy = line;

    if(strlen(line) && line[strlen(line) -1] == '\n'){
        line[strlen(line) - 1] = '\0';
    }

    if(!strlen(line) || *line == ';'){ /* ignore empty line or comment */
        isValid = false;
    } else {
        BLANKJMP(lineCopy); /* ignores blanks */
        if (strlen(lineCopy) == 0) { /* blank line ignores */
            isValid = false;
        }
    }

    if(isValid && strlen(line) > LINE_LEN){
        setErrorData(fd, "The length of the line is over the max");
        isValid = false;
    }
    return isValid;
}

/**
 * Function Name: lineParse
 * Function Description: parse the given line and making symbol-table and the basic code("word") value
 *  handle two passed the first make the basic code and label-table the second handle the externals
 * @param line the current line number
 * @param numberPass the curent pass for the current file
 * @param fd file information
 */
void lineParse(char *line, int numberPass, fileData * fd){
    char sym[LINE_OVER_MAX_SIZE] = "", directive[LINE_OVER_MAX_SIZE] = "";
    char *c = line;
    int symbolFlag = false, dir = CODE;
    unsigned int index = 0;
    int functionBreak = false;

    if(!isNeedCheckLine(line, fd)){
        functionBreak = true;
    }
    if(!functionBreak){
        if (numberPass == FIRST_PASS){ /* second pass, avoid */
            getSymbol(c, sym, &index, fd);
            if(strlen(sym)){
                symbolFlag = true;
            }
        }

        if(!fd->isHasError){
            getDirective(line, directive, &index, fd);
            if(!fd->isHasError && strlen(directive)){
                dir = checkDirective(directive, fd);
            }
        }

        if(!fd->isHasError && symbolFlag && dir != CODE) {
            if(dir == EXTERN || dir == ENTRY) {
                fprintf(stderr, "Warning: at line: %d label with extern is without meaning, ignore from label\n", fd->lineNumber);
            }else {
                insertSymbol(sym, dir); /* insert the symbol to symbol table */
            }
        }

        c = line + index;
        if(!fd->isHasError && ((numberPass == FIRST_PASS) || (numberPass == SECOND_PASS && dir == ENTRY))) {
            switch (dir) {
                case DATA:
                case STRING:
                    dirParse(c, dir, fd); /* parse the directive data (string or numbers) */
                    break;
                case ENTRY:
                case EXTERN:
                    getSymbolParam(c, sym, &index, fd);
                    if(!fd->isHasError) {
                        if(numberPass == FIRST_PASS && dir == ENTRY){ /* if is entry and first pass only check valid len */
                            checkSymbolLen(sym, fd);
                        }else {
                            checkSymbol(sym, numberPass, fd); /* check the symbol */
                        }
                        if(!fd->isHasError) {
                            if (dir == ENTRY) {
                                if(numberPass == SECOND_PASS) {
                                    entryMark(sym); /* enter the symbol to entries list */
                                }
                            } else { /* only in first around need to handle extern row */
                                insertSymbol(sym, EXTERN); /* insert to symbol table type extern */
                            }
                        }
                    }
                    break;
                case CODE:
                    if(symbolFlag){ /* symbol appeared */
                        BLANKJMP(c) /* ignore blanks */
                        cmdParse(sym, c, fd); /* parse the instruction with symbol */
                    }else {
                        cmdParse(NULL, c, fd); /* parse the instruction without symbol */
                    }
            }
        }
    }
}

/**
 * Function Name: dirParse
 * Function Description: parse the directive and their parameters
 * @param line the current line number
 * @param dir the type of directive
 * @param fd file information
 */
void dirParse(char *line, int dir, fileData * fd) {
    char *c = line; /* pointer to the line */

    BLANKJMP(c) /* ignore blanks */
    if(dir == DATA){
        dataCheck(c, fd); /* checks the data parameters */
    } else { /* string */
        stringCheck(c, fd); /* checks the string parameters */
    }
}

/**
 * Function Name: dataValidation
 * Function Description: checks if the string contain only valid chars
 * @param str the given string to check
 * @return if the string valid or not
 */
int dataValidation(char *str){
    char *c = str;
    int invalid = false;

    while (strlen(c) > 0 && !invalid){
        if(!isdigit(*c) && !isspace(*c) && *c != ',' && *c != '+' && *c != '-' && *c != '\0'){
            invalid = true;
        }
        c++;
    }
    return invalid;
}

/**
 * Function Name: dataCommaValidation
 * Function Description: checks the validity of the the comma in data parameters and turns on the relevant
 * @param sign flag to negative/positive character
 * @param comma flag to comma character
 * @param digit flag to digit character
 * @param blank flag to blank character
 * @param insertFlag flag to insert the collected value
 * @param fd file information
 */
/* Checks the validity of the the comma in data parameters and turns on the relevant */
void dataCommaValidation(int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd){
    if (!fd->isHasError && *comma) { /* two commas */
        setErrorData(fd,"In data directive parameters there is double commas");
    }
    if (!fd->isHasError && !(*digit)) { /* no digits */
        setErrorData(fd,"In data directive parameters there is no digits before comma");
    }
    /* updates the flags */
    *comma = *insertFlag = true; /* turn on flags */
    *blank = *digit = false; /* reset flags */
}

/**
 * Function: alternativeAtoi
 * Function Description: converts string to int
 * Alternative for the function atoi because sometimes atoi makes mistakes.
 * @param str the given string to convert to int
 * @return the converted string to int
 */
int alternativeAtoi(char* str) { 
    int sign = 1, result = 0, i = 0; 
    
    BLANKJMP(str) /* ignores blanks */

    /* determine the sign */
    if (str[i] == '-' || str[i] == '+') { 
        sign = 1 - 2 * (str[i++] == '-'); /* negetive assign -1 else 1 */
    } 
    /* moves the previous digit forward and insert the new one until end of str */
    while (isdigit(str[i])) {
        result = Dozen * result + (str[i++] - '0'); 
    }

    /* result with the sign */
    return result * sign; 
}

/**
 * Function: dataSignValidation
 * Function Description: checks the validity of the negative/positive sign in data parameters and turns on the relevant
 * @param currVal the given string to convert to int
 * @param sign flag to negative/positive character
 * @param comma flag to comma character
 * @param digit flag to digit character
 * @param blank flag to blank character
 * @param insertFlag flag to insert the collected value
 * @param fd file information
 */
void dataSignValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd){
    if (*blank && *digit) { /* no comma */
        setErrorData(fd, "In data directive parameters there is no comma seprates between numbers");
    }
    if (!fd->isHasError && *digit) { /* digit before sign */
        setErrorData(fd, "In data directive parameters negetive/positive sign not attached correctly");
    }
    if (!fd->isHasError && *sign) { /* two signs  */
        setErrorData(fd, "In data directive parameters there is double negetive/positive sign");
    }
    if (!fd->isHasError && *insertFlag) {
        insertData(alternativeAtoi(currVal)); /* insert the current value */
        *insertFlag = false; /* reset flag */
        *sign = true;
    }
}

/**
 * Function: dataDigitValidation
 * Function Description: checks the validity of the digit in data parameters and turns on the relevant
 * @param currVal the given string to convert to int
 * @param sign flag to negative/positive character
 * @param comma flag to comma character
 * @param digit flag to digit character
 * @param blank flag to blank character
 * @param insertFlag flag to insert the collected value
 * @param fd file information
 */
void dataDigitValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd){
    if (*digit && *blank) {/* blank separate digits */
        setErrorData(fd, "In data directive parameters blank cannot separate numbers");
    }
    if (!fd->isHasError && *insertFlag) { /* to insert data-list */
        insertData(alternativeAtoi(currVal)); /* insert to data-list the current value */
        *insertFlag = false; /* reset flag */
    }
    *sign = *comma = *blank = false; /* reset flags */
    *digit = true;
}

/**
 * Function: dataCheck
 * Function Description: checks the data parameters using flag for each type of valid char
 * @param pars the given parameters to check
 * @param fd file information
 */
void dataCheck(char *pars, fileData * fd){
    int digit = false, sign = false, comma = false, blank = false, insertFlag = true; /* flags to check the parameters */
    char *c = pars; /* pointer to the parameters */

    BLANKJMP(c) /* ignore blanks */
    if(!strlen(c)){ /* no params */
        setErrorData(fd, "No parameters for data directive");
    }
    if (dataValidation(c)){ /* invalid letter */
        setErrorData(fd, "Invalid letter for parameter data");
    }
    while (!fd->isHasError && *c != '\0'){ /* until end of string */
        if (!fd->isHasError && isdigit(*c)){ /* digit */
            /* checks validity and insert the current value */
            dataDigitValidation(c, &sign, &comma, &digit, &blank, &insertFlag, fd);
        } 
        else if(!fd->isHasError && *c != '\n' && isspace(*c)) { /* blank */
            if (sign) { /* sign before blank */
                setErrorData(fd, "There is a negetive/positive sign without a number");
            }
            blank = true;
        } 
        else if (!fd->isHasError && (*c == '+' || *c == '-')) { /* sign */
            dataSignValidation(c, &sign, &comma, &digit, &blank, &insertFlag, fd); /* checks validity */
        } 
        else if (!fd->isHasError && *c == ',') { /* comma */
            dataCommaValidation(&sign, &comma, &digit, &blank, &insertFlag, fd); /* checks validity */
        }
        c++; /* next char */
    }
    if (!fd->isHasError && comma) { /* last char is comma */
        setErrorData(fd, "Data parameters can't end with comma");
    }
}

/**
 * Function: stringCheck
 * @param pars the given parameters to check
 * @param fd file information
 */
void stringCheck(char *pars, fileData * fd) {
    char *c = pars; /* pointer to parameters */
    int len = strlen(pars), i;
    char * stringStart, * stringEnd;

    BLANKJMP(c) /* ignore blanks */
    if(*c != '\"') { /* the first char isn't quotes */
        setErrorData(fd, "String parameter must start with quotes");
    }
    if(!fd->isHasError) {
        stringStart = c;
        c = pars + len -1; /* point to end of line */
        for(; c != stringStart && *c != '\"' && isspace(*c); c--); /* revert string from end to find quotes end of string */
        if(c == stringStart){
            setErrorData(fd, "Directive data must end in quotes");
        }else if(*c != '\"'){
            setErrorData(fd, "String parameter must end with quotes");
        }else {
            stringEnd = --c; /* address without quotes end string */
            stringStart++; /* ignore quotes start string */
            c = stringStart; /* return to the start */
            for(i = 0; i < stringEnd - stringStart + 1; i++){ /* each iteration insert the cuurent value */
                insertData(c[i]);
            }
            insertData('\0'); /* the string terminator */
        }
    }
}


/**
 * Function: checkDirective
 * Function Description: checks the directive and return the number of directive
 * @param dir the directive type
 * @param fd file information
 */
int checkDirective(char *dir, fileData * fd){
    int res = NOT_FOUND;

    if(!strcmp(dir,"data"))
        res = DATA;
    else if(!strcmp(dir,"string"))
        res = STRING;
    else if(!strcmp(dir,"entry"))
        res = ENTRY;
    else if(!strcmp(dir,"extern"))
        res = EXTERN;
    else
        setErrorData(fd, "Invalid directive");
    return res;
}
