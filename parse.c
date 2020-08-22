/* updated */

/* parse.c contain all the function that parse info from file */
#include "parse.h"

/**
 * function save the parameter of symbol
 * @param line
 * @param save
 * @param nextIndex
 */
void getSymbolParam(char * line, char * save, unsigned int * nextIndex, fileData * fd){
    char copy[SYMBOL_LEN], *temp, *c;
    unsigned int index = 0;

    save[0] = '\n';
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

void getSymbol(char * str, char * save, unsigned int * nextIndex, fileData * fd){
    char * temp, copy[LINE_OVER_MAX_SIZE];

    strcpy(copy, str);
    temp = copy;
    BLANKJMP(temp) /* ignore blanks */
    *nextIndex = temp - copy;
    temp = strtok(temp, ":");
    if(strlen(temp) > SYMBOL_LEN){
        setErrorData(fd, "The length of symbol over the max");
        save[0] = '\0';
    }else {
        if (strlen(str + *nextIndex) == strlen(temp)) {
            save[0] = '\0';
        } else {
            strcpy(save, temp);
            *nextIndex += strlen(temp);
            (*nextIndex)++; /* ignore : */
            checkSymbol(save, FIRST_PASS, fd); /* check the symbol */
        }
    }
}

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

/*
* The function lineParse parse the given line and making symbol-table and the basic code("word") value
* handle two passed the first make the basic code and label-table the second handle the externals
*/
void lineParse(char *line, int numberPass, fileData * fd){
    char sym[SYMBOL_LEN] = "", directive[DIRECTIVE_LEN] = "";
    char *c = line;
    int symbolFlag = false, dir = CODE;
    unsigned int index = 0;

    if(!isNeedCheckLine(line, fd)){
        return;
    }

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
        insertSymbol(sym, dir); /* insert the symbol to symbol table */
    }
    c = line + index;
    if(!fd->isHasError && ((numberPass == FIRST_PASS && dir != ENTRY) || (numberPass == SECOND_PASS && dir == ENTRY))) {
        switch (dir) {
            case DATA:
            case STRING:
                dirParse(c, dir, fd); /* parse the directive data (string or numbers) */
                break;
            case ENTRY:
            case EXTERN:
                getSymbolParam(c, sym, &index, fd);
                if(!fd->isHasError) {
                    checkSymbol(sym, numberPass, fd); /* check the symbol */
                    if(!fd->isHasError) {
                        if (dir == ENTRY) {
                            entryMark(sym); /* enter the symbol to entries list */
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

/* The function dirParse parse the directive and their parameters */
void dirParse(char *line, int dir, fileData * fd) {
    char *c = line; /* pointer to the line */
    BLANKJMP(c) /* ignore blanks */
    if(dir == DATA){
        dataCheck(c, fd); /* checks the data parameters */
    } else { /* string */
        stringCheck(c, fd); /* checks the string parameters */
    }
}

/* Checks if the string contain only valid chars */
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

/* Checks the validity of the negetive/positive sign in data parameters and turns on the relevant */
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
        insertData(atoi(currVal)); /* insert the current value */
        *insertFlag = false; /* reset flag */
        *sign = true;
    }
}

/* Checks the validity of the digit in data parameters and turns on the relevant */
void dataDigitValidation(char *currVal, int *sign, int *comma, int *digit, int *blank, int *insertFlag, fileData * fd){
    if (*digit && *blank) {/* blank separate digits */
        setErrorData(fd, "In data directive parameters blank cannot separate numbers");
    }
    if (!fd->isHasError && *insertFlag) { /* to insert data-list */
        insertData(atoi(currVal)); /* insert to data-list the current value */
        *insertFlag = false; /* reset flag */
    }
    *sign = *comma = *blank = false; /* reset flags */
    *digit = true;
}

/* The function dataCheck checks the data parameters using flag for each type of valid char */
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

/* The function stringCheck check the string directive parameters and insert to data list */
void stringCheck(char *pars, fileData * fd) {
    char *c = pars; /* pointer to parameters */
    int len = strlen(pars);
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
            strncpy(c, stringStart, stringEnd - stringStart + 1); /* copy real string */
            for(; *c; c++){
                insertData(*c);
            }
            insertData('\0'); /* the string terminator */
        }
    }
}

/* The function checkDirective check the directive and return the number of directive */
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
