/* updated */

/* symbolHandler.c contain all the symbols and symbol-table handling */
#include "symbolHandler.h"

/* The function checkSymbol check if the symbol valid */
void checkSymbol(char *symbol, int numberPass, fileData * fd){
    if(numberPass == FIRST_PASS) { /* for first pass */
        syntaxCheck(symbol, fd); /* check  the syntax */
    }
    if(!fd->isHasError) {
        existence(symbol, numberPass, fd); /* check existence */
    }
}

/* The function syntaxCheck checks if the syntax of the symbol valid */
void syntaxCheck(char *symbol, fileData * fd){
    char *c = symbol; /* pointer to symbol */
    int i;

    if(!isalpha(*c)) /* not started with alpha */
        setErrorData(fd, "Symbol must started with letter");

    if(!fd->isHasError && strlen(c) > SYMBOL_LEN) /* symbol length check */
        setErrorData(fd, "The length of symbol over the max");

    if(!fd->isHasError) {
        for (i = 1; i < strlen(symbol) && !fd->isHasError; i++) /* checks the entire symbol */
            if (!isalpha(*(c + i)) && !isdigit(*(c + i))) { /* number or alpha */
                setErrorData(fd, "Symbol must contain only letter or numbers");
            }
    }
    
    /* all the saved words in our program */
    if(!fd->isHasError && !(
        strcmp(symbol,"r0") && strcmp(symbol,"r1") && strcmp(symbol,"r2") && strcmp(symbol,"r3") && strcmp(symbol,"r4") &&
        strcmp(symbol,"r5") && strcmp(symbol,"r6") && strcmp(symbol,"r7") &&
        strcmp(symbol,"mov") && strcmp(symbol,"cmp") && strcmp(symbol,"add") && strcmp(symbol,"sub") &&
        strcmp(symbol,"lea") && strcmp(symbol,"clr") && strcmp(symbol,"not") && strcmp(symbol,"inc") &&
        strcmp(symbol,"dec") && strcmp(symbol,"jmp") && strcmp(symbol,"bne") && strcmp(symbol,"jsr") &&
        strcmp(symbol,"red") && strcmp(symbol,"prn") && strcmp(symbol,"rts") && strcmp(symbol,"stop") &&
        strcmp(symbol,"string") && strcmp(symbol,"data") && strcmp(symbol,"extern") && strcmp(symbol,"entry")
    )){
        setErrorData(fd, "Symbol name can't be reserved word");
    }
}

/* The function existence check if the symbol exist, for first pass its an error for second its valid */
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
    if(numberPass == SECOND_PASS && isFound != true) /* not found symbol */
        setErrorData(fd, "The symbol mentioned and not defined");
}

/* The function updateStable is updating the symbol table after the first pass */
void updateStable(void)
{
    symbolNode *curr = symHead;
    if(symHead == NULL) /* no symbol table */
        return;
    while(curr) /* until null */
    {
        if(curr->type & SYMBOL_DATA) /* if the symbol type data we update the value */
            curr->value += IC;
        curr = curr->next; /* next node */
    }
}

/* The function insertSymbol is insert the give symbol name and the type if have one to symbol table */
void insertSymbol(char *symbol, int dir){
    if(symHead != NULL)
        addSymbol(symbol, dir); /* add symbol to symbol-table */
    else 
        creatTable(symbol, dir); /* create symbol table */
}

/* The function addSymbol is insert the given symbol to symbol table */
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

/* The function createTable is create a symbol table and insert the given symbol */
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
    symTail = symHead;
}

void resetSym(symbolNode *node){
    node->value = 0;
    node->type = 0;
    node->next = NULL;
    node->prev = NULL;
}

/* The function entryMark mark the entry symbols in symbol table */
void entryMark(char *sym)
{
    symbolNode *curr = symHead;
    while(curr)
    {
        if(!strcmp(curr->name,sym)) /* found match */
            curr->type |= SYMBOL_ENTRY; /* entry flag on */
        curr = curr->next;
    }
    
}

/* The function insertExt insert the given symbol to extern list for the extern file creation */
void insertExt(char* sym, unsigned long int adrs)
{
    if(extHead == NULL)
        createExt(sym,adrs); /* create list */
    else
        addExt(sym,adrs); /* add to exist list */
}

/* The function createExt create extern-list and insert the given symbol */
void createExt(char* symbol, unsigned long int adrs){
    extHead = (externNode*)malloc(sizeof(externNode)); /* memo allocation */

    if(extHead == NULL){
        allocateFailed();
    }
    extHead->address = adrs; /* insert the given address */
    strcpy(extHead->sym,symbol); /* insert the give name */
    extHead->next = NULL; /* set the next/prev */
    extHead->prev = NULL;
    extTail = extHead;
}

/* The function addExt insert the given symbol to extern list */
void addExt(char* symbol, unsigned long int adrs){
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