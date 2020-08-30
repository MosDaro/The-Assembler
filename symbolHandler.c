/**
 * Authors: Moisei Shkil & Shimon Biton
 * Date: 29/08/20
 * File Name: symbolHandler.c
 * File Description: contain all the symbols and symbol-table handling.
 */
#include "symbolHandler.h"

/**
 * Function: checkSymbol
 * Function Description: check if the symbol valid
 * @param symbol the given symbol name
 * @param numberPass the number of the pass
 * @param fd file information
 */
void checkSymbol(char *symbol, int numberPass, fileData *fd){
    if(checkSymbolLen(symbol, fd)){
        if(numberPass == FIRST_PASS) { /* for first pass */
            syntaxCheck(symbol, fd); /* check  the syntax */
        }
        if(!fd->isHasError) {
            existence(symbol, numberPass, fd); /* check existence */
        }
    }
}

/**
 * Function: checkSymbolLen
 * Function Description: check if the symbol length is valid
 * @param symbol the given symbol name
 * @param fd file information
 * @return if the symbol valid of not
 */
int checkSymbolLen(char * symbol, fileData *fd){
    int isValid = true;

    if(strlen(symbol) > SYMBOL_LEN){
        isValid = false;
        setErrorData(fd, "The length of symbol over the max");
    }
    return isValid;
}

/**
 * Function: syntaxCheck
 * Function Description: checks if the syntax of the symbol valid
 * @param symbol the given symbol name
 * @param fd file information
 */
void syntaxCheck(char *symbol, fileData * fd){
    extern char * cmdList[];
    extern char * regList[];
    char *c = symbol; /* pointer to symbol */
    int i;

    if(!isalpha(*c)) /* not started with alpha */
        setErrorData(fd, "Symbol must started with letter");

    if(!fd->isHasError && checkSymbolLen(c, fd)){
        for (i = 1; i < strlen(symbol) && !fd->isHasError; i++) /* checks the entire symbol */
            if (!isalpha(*(c + i)) && !isdigit(*(c + i))) { /* number or alpha */
                setErrorData(fd, "Symbol must contain only letter or numbers");
            }
    }

    /* all the saved words in our program */
    if(!fd->isHasError){
        for(i = 1; i < NUM_OF_CMD && !fd->isHasError; i++){
            if(!strcmp(cmdList[i], symbol)){
                setErrorData(fd, "Symbol name can't be reserved word");
            }
        }
        for(i = 0; i < NUM_OF_REG && !fd->isHasError; i++){
            if(!strcmp(regList[i], symbol)){
                setErrorData(fd, "Symbol name can't be reserved word");
            }
        }
    }
}

/**
 * Function: existence
 * Function Description: checks if the symbol exist, for first pass its an error for second its valid
 * @param symbol the given symbol name
 * @param numberPass the number of the pass
 * @param fd file information
 */
void existence(char *symbol, int numberPass, fileData * fd) {
    int isFound = -1;
    symbolNode *curr = symHead; /* pointer to head of symbol table */

    while(curr && isFound != true) { /* until end of symbol table */
        if(numberPass == FIRST_PASS && !strcmp(curr->name,symbol)) { /* first pass found match */
            if(curr->type & SYMBOL_EXTERNAL){
                setErrorData(fd, "You can't declare entry symbol because declared as external");
            }else {
                setErrorData(fd, "In this file symbol has already defined");
            }
            isFound = true;
        }
        if(numberPass == SECOND_PASS && !strcmp(curr->name,symbol)) { /* second pass found match */
            isFound = true;
        }
        curr = curr->next; /* next node */
    }
    if(numberPass == SECOND_PASS && isFound != true) { /* not found symbol */
        setErrorData(fd, "The symbol mentioned and not defined");
    }
}

/**
 * Function: updateStable
 * Function Description: updates the symbol table after the first pass
 */
void updateStable(void) {
    symbolNode *curr = symHead;
    if(symHead == NULL) /* no symbol table */
        return;
    while(curr) { /* until null */
        if(curr->type & SYMBOL_DATA) /* if the symbol type data we update the value */
            curr->value += IC;
        curr = curr->next; /* next node */
    }
}

/**
 * Function: insertSymbol
 * Function Description: inserts the give symbol name and the type if have one to symbol table
 * @param symbol the given symbol name
 * @param dir the type of symbol
 */
void insertSymbol(char *symbol, int dir){
    if(symHead != NULL)
        addSymbol(symbol, dir); /* add symbol to symbol-table */
    else 
        creatTable(symbol, dir); /* create symbol table */
}

/**
 * Function: addSymbol
 * Function Description: inserts the given symbol to symbol table
 * @param symbol the given symbol name
 * @param dir the type of symbol
 */
void addSymbol(char *symbol, int dir){
    symbolNode *temp;

    temp = (symbolNode*)malloc(sizeof(symbolNode)); /* memory allocation for node */
    if(!temp){
        allocateFailed();
    }
    resetSym(temp);
    strcpy(temp->name,symbol); /* insert the name */
    if(dir == DATA || dir == STRING) { /* DATA */
        temp->value = DC; /* dc value */
        temp->type |= SYMBOL_DATA; /* data bit on */
    }
    else if(dir == EXTERN) { /* extern type */
        temp->value = 0; /* non value */
        temp->type |= SYMBOL_EXTERNAL; /* extern bit on */
    }
    else if(dir == CODE) { /* code type */
        temp->type |= SYMBOL_CODE; /* code bit on */
        temp->value = IC; /* IC value */
    }
    temp->next = symHead; /* set the node */
    symHead->prev = temp;
    symHead = temp;
}

/**
 * Function: creatTable
 * Function Description: create a symbol table and insert the given symbol
 * @param symbol the given symbol name
 * @param dir the type of symbol
 */
void creatTable(char *symbol, int dir) {
    symHead = (symbolNode*)malloc(sizeof(symbolNode)); /* memory allocation for the head of table */
    if(!symHead){
        allocateFailed();
    }
    resetSym(symHead);
    strcpy(symHead->name, symbol); /* insert the name */
    if(dir == DATA || dir == STRING) {
        symHead->value = DC;
        symHead->type |= SYMBOL_DATA; /* data bit on */
    }
    else if(dir == EXTERN) {
        symHead->type |= SYMBOL_EXTERNAL; /* external bit on */
    }
    else if(dir == CODE) {
        symHead->type |= SYMBOL_CODE;
        symHead->value = IC;
    }
    symHead->next = NULL; /* node set */
    symHead->prev = NULL;
}

/**
 * Function: resetSym
 * Function Description: reset the symbol nodes' values of linked list
 * @param node the given node to reset
 */
void resetSym(symbolNode *node){
    node->value = 0;
    node->type = 0;
    node->next = NULL;
    node->prev = NULL;
}

/**
 * Function: entryMark
 * Function Description: marks the entry symbols in symbol table
 * @param sym the given symbol to insert
 */
void entryMark(char *sym) {
    symbolNode *curr = symHead;

    while(curr)
    {
        if(!strcmp(curr->name,sym)) /* found match */
            curr->type |= SYMBOL_ENTRY; /* entry flag on */
        curr = curr->next;
    }
    
}

/**
 * Function: insertExt
 * Function Description: inserts the given symbol to extern list for the extern file creation
 * @param sym the given symbol to insert
 * @param adrs the addres of symbol
 */
void insertExt(char* sym, unsigned long int adrs) {
    if(extHead == NULL)
        createExt(sym,adrs); /* create list */
    else
        addExt(sym,adrs); /* add to exist list */
}

/**
 * Function: createExt
 * Function Description: creates extern-list and insert the given symbol
 * @param symbol the given symbol to insert
 * @param adrs the addres of symbol
 */
void createExt(char* symbol, unsigned long int adrs){
    extHead = (externNode*)malloc(sizeof(externNode)); /* memo allocation */

    if(extHead == NULL){
        allocateFailed();
    }
    extHead->address = adrs; /* insert the given address */
    strcpy(extHead->sym,symbol); /* insert the give name */
    extHead->next = NULL; /* set the next/prev */
    extHead->prev = NULL;
}

/**
 * Function: addExt
 * Function Description: insert the given symbol to extern list
 * @param symbol the given symbol to insert
 * @param adrs the addres of symbol
 */
void addExt(char *symbol, unsigned long int adrs){
    externNode *node = (externNode*)malloc(sizeof(externNode)); /* memo allocation */

    if(node == NULL){
        allocateFailed();
    }
    node->address = adrs; /* insert the address */
    strcpy(node->sym,symbol); /* insert the symbol name */
    node->next = extHead; /* sets next/prev */
    node->prev=NULL;
    extHead->prev = node;
    extHead = node;
}

/**
 * Function: externExistance
 * Function Description: Checks the existance of the given symbol in extern table
 * @param symbol the given symbol to check
 * @return if found or not
 */
int externExistance(char *symbol){
    int isFound = false;
    externNode *curr = extHead; /* pointer to head of extern table */
    while(curr && !isFound) { /* until end of symbol table */
        if(!strcmp(curr->sym,symbol)) { /* first pass found match */
            isFound = true;
        }
        curr = curr->next; /* next node */
    }
    return isFound;
}