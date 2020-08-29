/*
 * Authors: Moisei Shkil & Shimon Biton
 * Date: 29/08/20
 * File Name: dataStructs.c
 * File Description: contain all the structures maintaining.
 */

#include "dataStructs.h"


/**
 * Function: insertData
 * Function Description: insert the given value to data list
 * 
 * @param value the value to insert
 */
void insertData(unsigned long int value){
    if(dataHead == NULL)
        createData(value); /* create data list */
    else
        addData(value); /* insert to exist data list */
}


/**
 * Function: createData
 * Function Description: creates data list and insert the given value to the list
 * 
 * @param value the value to insert
 */
void createData(unsigned long int value){
    dataHead = (dataNode*)malloc(sizeof(dataNode));

    if(dataHead == NULL){
        allocateFailed();
    }
    dataHead->val = (unsigned char*)malloc(NUM_BITS); /* allocation of 24 bits and set char pointer */
    if(dataHead->val == NULL){
        allocateFailed();
    }
    resetAlocVal(dataHead->val);
    setVal(dataHead->val,value); /* insert the value */
    dataHead->next = NULL; /* sets the next/prev */
    dataHead->prev = NULL;
    DC++; /* data counter increase */
}


/**
 * Function: addData
 * Function Description: insert the given value to data list
 * 
 * @param value the value to insert
 */
void addData(unsigned long int value){
    dataNode *node = (dataNode*)malloc(sizeof(dataNode));

    if(node == NULL){
        allocateFailed();
    }
    node->val = (unsigned char*)malloc(NUM_BITS);
    if(node->val == NULL){
        allocateFailed();
    }
    resetAlocVal(node->val);
    setVal(node->val,value); /* insert the value */
    node->next = dataHead;
    dataHead->prev = node; 
    node->prev = NULL;
    dataHead = node;
    DC++; /* data counter increase */
}

/**
 * Function: insertCmd
 * Function Description: insert the given value to instruction list and if the value unknowen to fix list
 * 
 * @param value the value to insert
 * @param address the address to save
 * @param sym the symbol name
 * @param fd
 */
void insertCmd(unsigned long int value, unsigned long int address, char *sym, fileData * fd) {
    if(address && sym){ /* the address and the symbol name not empty */
        if(fixHead == NULL)
            createFix(address, sym, fd); /* create fix list */
        else
            addFix(address, sym, fd); /* insert address and symbol to exist list */
    }
    if(!fd->isHasError) {
        if (cmdHead == NULL) /* if cmd list empty */
            createCmd(value);
        else
            addCMD(value);
    }
}

/**
 * Function: createCmd
 * Function Description: creates list of instructions code and set the value
 * 
 * @param value the value to insert
 */
void createCmd(unsigned long int value){
    cmdHead = (cmdNode*)malloc(sizeof(cmdNode)); /* memo allocation */

    if(cmdHead == NULL){
        allocateFailed();
    }
    cmdHead->val = (unsigned char*)malloc(NUM_BITS); /* allocate 24 bits */
    if(cmdHead->val == NULL) {
        allocateFailed();
    }
    resetAlocVal(cmdHead->val);
    setVal(cmdHead->val,value);
    cmdHead->next = NULL; /* set next/prev */
    cmdHead->prev = NULL;
    IC++; /* increase instruction counter */
}

/**
 * Function: addCMD
 * Function Description: adds new node to cmd list and insert the value
 * 
 * @param value the value to insert
 */
void addCMD(unsigned long int value) {
    cmdNode *node = (cmdNode*)malloc(sizeof(cmdNode)); /* memo allocation */

    if(node == NULL){
        allocateFailed();
    }
    node->val = (unsigned char*)malloc(NUM_BITS); /* allocate 24 bits */
    if(node->val == NULL) {
        allocateFailed();
    }
    resetAlocVal(node->val);
    setVal(node->val,value); /* insert the value */
    node->next = cmdHead; /* set next/ prev */
    node->prev = NULL;
    cmdHead->prev = node;
    cmdHead = node;
    IC++; /* increase instruction counter */
}

/**
 * Function: createFix
 * Function Description: create fix list and insert the given values to the head
 * 
 * @param address the address to save
 * @param sym the symbol name
 * @param fd
 */
void createFix(unsigned long int address, char *sym, fileData * fd) {
    fixHead = (fixNode*)malloc(sizeof(fixNode)); /* memo allocation */
    if(fixHead == NULL) {
        allocateFailed();
    }
    fixHead->address = address; /* insert the address */
    strcpy(fixHead->symbol,sym); /* insert the symbol name */
    fixHead->line = fd->lineNumber; /* insert the number of line where the symbol appearde */
    fixHead->next = NULL; /* set next/prev */
    fixHead->prev = NULL;
    fixHead->nodesCount = 1; /* number of nodes */
}

/**
 * Function: addFix
 * Function Description: inserts the given address and symbol to fix list
 * 
 * @param address the address to save
 * @param sym the symbol name
 * @param fd
 */
void addFix(unsigned long int address, char *sym, fileData * fd){
    fixNode *node = (fixNode*)malloc(sizeof(fixNode));
    
    if(node == NULL){
        allocateFailed();
    }
    node->address = address; /* insert the address */
    strcpy(node->symbol,sym); /* insert the symbol */
    node->line = fd->lineNumber; /* insert the line number */
    node->next = fixHead; /* set the next/prev */
    fixHead->prev = node; /* set next/prev */
    node->nodesCount = fixHead->nodesCount + 1; /* number of nodes */
    fixHead = node;
}

/**
 * Function: makeMask
 * Function Description: returns mask with given bit number on
 * 
 * @param bitNum the number of bit to turn on
 * @return the edited mask
 */
int makeMask(int bitNum){
    int mask = 1;

    mask <<= bitNum; /* moves left given number of times */
    return mask; 
}

/**
 * Function: setVal
 * Function Description: insert the given int value to the given pointer 24 bits field
 * 
 * @param word the word to set
 * @param value the value to insert
 */
void setVal(unsigned char* word, unsigned long int value){
    unsigned char *pWord = word; /* pointer to the start of the field */
    int mask = FIRST_FIELD & value; /* mask the turned on bits */
    *pWord |= mask; /* insert them */

    pWord++; /* next field */
    mask = SECOND_FIELD & value; /* mask the second field */
    mask >>= ONE_FIELD; /* set to the first field */
    *pWord |= mask; /* insert */
    pWord++; /* next field */
    mask = THIRD_FIELD & value; /* mask the third field */
    mask >>= ONE_FIELD; /* move to right spot */
    mask >>= ONE_FIELD;
    *pWord |= mask; /* insert */
}

/**
 * Function: setInt
 * Function Description: gets pointer of word and insert the value to int variable
 * 
 * @param word the given word to insert
 * @return the result of word inserted to int
 */
int setInt(unsigned char* word){
    unsigned char *pWord = word; /* point to start of word */
    unsigned int result = 0;
    int mask = -1 & *pWord; /* mask the turned on  bits */

    result |= mask; /* insert them */
    pWord++; /* next field */
    mask = FIRST_FIELD & *pWord; /* mask the second field */
    mask <<= ONE_FIELD; /* move to the right spot */
    result |= mask; /* insert */
    pWord++; /* next field */
    mask = -1 & *pWord; /* mask the turned on field */
    mask <<= ONE_FIELD; /* move to right spot */
    mask <<= ONE_FIELD;
    result |= mask; /* insert */
    return result;
}

/**
 * Function: clearARE
 * Function Description: moves all the bits 3 spots left to make space for ARE
 * 
 * @param word the word to fix ARE
 */
void clearARE(unsigned char *word){
    int consMask;
    unsigned char *p = word+FIELDS-1; /* set to be last field */
    int mask = makeMask(FIFTH_FNCT); /* turn on bit number 7 */

    mask |= makeMask(FORTH_FNCT); /* turn on bit number 6 */
    mask |= makeMask(THIRD_FNCT); /* turn on bit number 5 */
    consMask = mask; /* to save the mask */
    *p <<= FIELDS; /* move bits 3 left */
    mask &= *(p-1); /* mask the second field */
    mask >>= MOVE_TO_FIRST; /* set the mask to the first bits */
    *p |= mask; /* insert the on bits */
    mask = consMask; /* reset the mask */
    p = word+1; /* point to second field */
    *p <<= FIELDS; /* move bits 3 left */
    p = word; /* set the pointer to first field */
    mask &= *p; /* mask the first field */
    mask >>= MOVE_TO_FIRST; /* set the mask to the first bits */
    *(p+1) |= mask; /* insert the on bits */
    *p <<= FIELDS; /* make space for ARE */
}

/**
 * Function: resetAlocVal
 * Function Description: reset all the 24 bits of given pointer
 * 
 * @param val the value to reset
 */
void resetAlocVal(unsigned char *val){
    unsigned char *pointer = val;
    *pointer = 0;
    *(++pointer) = 0;
    *(++pointer) = 0;
}