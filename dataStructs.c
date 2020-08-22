/* updated */

/* dataStructs.c contain all the structure handling */
#include "dataStructs.h"

/* The function insertData is insert the given value to data list */
void insertData(int value){
    if(dataHead == NULL)
        createData(value); /* create data list */
    else
        addData(value); /* insert to exist data list */
}

/* The function createData create data list and insert the given value to the list */
void createData(int value){
    dataHead = (dataNode*)malloc(sizeof(dataNode));

    if(dataHead == NULL){
        allocateFailed();
    }
    dataHead->val = (unsigned char*)malloc(NUM_BITS); /* allocation of 24 bits and set char pointer */
    if(dataHead->val == NULL){
        allocateFailed();
    }
    *(dataHead->val) = 0;
    setVal(dataHead->val,value); /* insert the value */
    dataHead->next = NULL; /* sets the next/prev */
    dataHead->prev = NULL;
    dataTail = dataHead;
    DC++; /* data counter increase */
}

/* The function addData insert the given value to data list */
void addData(int value){
    dataNode *node = (dataNode*)malloc(sizeof(dataNode));

    if(node == NULL){
        allocateFailed();
    }
    node->val = (unsigned char*)malloc(NUM_BITS);
    if(node->val == NULL){
        allocateFailed();
    }
    *(node->val) = 0;
    setVal(node->val,value); /* insert the value */
    node->next = dataHead;
    dataHead->prev = node; 
    node->prev = NULL;
    dataHead = node;
    DC++; /* data counter increase */
}

/* The function insertCmd insert the given value to instruction list and if the value unknowen to fix list 2 */
void insertCmd(int value, unsigned long int address, char *sym, fileData * fd) {
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

/* The function createCMD create list of instructions code and set the value */
void createCmd(int value){
    cmdHead = (cmdNode*)malloc(sizeof(cmdNode)); /* memo allocation */

    if(cmdHead == NULL){
        allocateFailed();
    }
    cmdHead->val = (unsigned char*)malloc(NUM_BITS); /* allocate 24 bits */
    if(cmdHead->val == NULL) {
        allocateFailed();
    }
    *(cmdHead->val) = 0;
    setVal(cmdHead->val,value);
    cmdHead->next = NULL; /* set next/prev */
    cmdHead->prev = NULL;
    cmdTail = cmdHead;
    IC++; /* increase instruction counter */
}

/* The function addCMD add new node to cmd list and insert the value */
void addCMD(int value) {
    cmdNode *node = (cmdNode*)malloc(sizeof(cmdNode)); /* memo allocation */

    if(node == NULL){
        allocateFailed();
    }
    node->val = (unsigned char*)malloc(NUM_BITS); /* allocate 24 bits */
    if(node->val == NULL) {
        allocateFailed();
    }
    *(node->val) = 0;
    setVal(node->val,value); /* insert the value */
    node->next = cmdHead; /* set next/ prev */
    node->prev = NULL;
    cmdHead->prev = node;
    cmdHead = node;
    IC++; /* increase instruction counter */
}

/* The function createFix is create fix list and insert the given values to the head */
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
    fixTail = fixHead;
    ind++; /* number of node increase */
}

/* The function addFix insert the given address and symbol to fix list */
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
    fixHead = node;
    ind++; /* number of nodes increase */
}

/* The function makeMask is return mask with given bit number on */
int makeMask(int bitNum){
    int mask = 1;

    mask <<= bitNum; /* move left given number of times */
    return mask; 
}

/* The function setVal insert the given int value to the given pointer 24 bits field */
void setVal(unsigned char* word, int value){
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

/* The function setInt get pointer of word and insert the value to int variable */
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

/* The function clearARE move all the bits 3 spots left to make space for ARE */
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