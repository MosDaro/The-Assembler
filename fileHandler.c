/* fileHandler.c is contain all the files handling */
#include "fileHandler.h"

/**
 * Function: resetFileDataStruct
 * Function Description: reset the values of the struct
 * 
 * @param err the error message
 * @param fileName the given file name
 */
void resetFileDataStruct(fileData * err, char * fileName){
    strcpy(err->fileName, fileName);
    err->lineNumber = 1;
    err->msg[0] = '\0';
    err->isHasError = false;
}

/**
 * Function: readFile
 * Function Description: handle the given file, parse him and make an output files
 * 
 * @param file the file name
 */
void readFile(char *file){
    char fileName[FILE_NAME_LEN] = "";
    fileData err;
    FILE *fp;

    strcpy(fileName, file); /* insert the given file */
    strcat(fileName, ".as"); /* add the suffix to the file */
    FILE_OPEN(fp, fileName, "r") /* open the current file with the given style and sets the pointer */
    resetFileDataStruct(&err, fileName);
    if(firstPass(fp, &err) && !err.isHasError) { /* call the first pass if return 0 there is an errors */
        updateStable(); /* update the symbol table values */
        if(secondPass(fp, &err)) /* call the second pass if return 1 make output files */
            createFiles(file); /* create the output files */
    }
    freeAll(); /* free all the allocated memory */
    fclose(fp); /* close the current file */
}

/**
 * Function: readFile
 * Function Description: checks the file line by line and making the basic machine code and symbol-table
 * 
 * @param fp file pointer
 * @param fd follow the data about the file, line and errors
 * @return if error accours return 0 else 1
 */
int firstPass(FILE *fp, fileData * fd){
    char line[LINE_OVER_MAX_SIZE]="";
    int isPassOk = true;
    IC = 0; /* set the instruction count */
    DC = 0; /* set the data counter */

    while(fgets(line, LINE_OVER_MAX_SIZE, fp) != NULL) { /* read the current file line by line and parse the line */
        lineParse(line, FIRST_PASS, fd); /* parse the line, "1" means first pass */
        fd->lineNumber++;
        if(fd->isHasError){
            isPassOk = false;
        }
        fd->isHasError = false;
    }
    rewind(fp); /* sets the pointer to the start */
    return isPassOk;
}

/**
 * Function: secondPass
 * Function Description: completes the missing code and tag the extern symbols
 * 
 * @param fp file pointer
 * @param fd follow the data about the file, line and errors
 * @return if error accours return 0 else 1
 */
int secondPass(FILE *fp, fileData * fd){
    char line[LINE_LEN] = "";
    int isPassOk = true;

    fd->lineNumber = 1; /* reset line count */
    while(fgets(line,LINE_LEN,fp)!=NULL) {/* read line by line to send to parse */
        lineParse(line,SECOND_PASS, fd); /* parse the line for second pass */
        fd->lineNumber++;
        if(fd->isHasError){
            isPassOk = false;
        }
    }
    if(isPassOk) {
        insertMissing(fd); /* insert the missing code info */
    }

    return isPassOk; /* passed the checks */
}


/**
 * Function: insertMissing
 * Function Description: insert all the missing info code(direct address, externs etc..) 
 *  using the fix-list that contain all the cells that missing info
 * 
 * @param fd follow the data about the file, line and errors
 */
void insertMissing(fileData * fd){
    int i, numberOfNode;

    fixNode *curr = fixHead; /* set pointer to fix-list */
    numberOfNode = fixHead->nodesCount;
    for(i = 0; i < numberOfNode && curr; i++){ /* run on fix-list backward */
        updateOp(curr, fd); /* update the missing code */
        curr = curr->next; /* set the prev */
    }
}

/**
 * Function: updateOp
 * Function Description: updates the code from given address using symbol-table 
 * 
 * @param fixNode the poniter to linked list of unkown symbols
 * @param fd follow the data about the file, line and errors
 */
void updateOp(fixNode * fixNode, fileData * fd){
    int j = 0, functionBreak = false;
    int i = fixNode->address;
    char * sym = fixNode->symbol;

    symbolNode *curr = symHead; /* sets the pointer to head of symbol table */
    cmdNode *cmdCurr = getCmdTail();

    if(symHead == NULL || cmdCurr == NULL){
        functionBreak = true;
    }
    if(!functionBreak){
        while(j < i){
            cmdCurr = cmdCurr->prev;
            j++;
        }
    }
    fd->lineNumber = fixNode->line;
    if(!functionBreak && *sym == '&') { /* first letter is & */
        existence(sym + 1, SECOND_PASS, fd); /* check existence of the given symbol without the first letter */
    } else if(!functionBreak)
        existence(sym,SECOND_PASS, fd); /* check existence */
    if(!functionBreak && !fd->isHasError) {
        while (curr) {/* search the given symbol in symbol table */
            if (!functionBreak && !(curr->type & SYMBOL_EXTERNAL) && !strcmp(curr->name, sym)) { /* not extern and match */
                setVal(cmdCurr->val, curr->value + IC_START); /* insert the value to the word */
                clearARE(cmdCurr->val); /* move all the bits 3 to the left */
                *(cmdCurr->val) |= makeMask(R_BIT); /* turn the R(relocatble) bit */
            }
            if (!functionBreak && (curr->type & SYMBOL_EXTERNAL) && !strcmp(curr->name, sym)) { /* external and match */
                *(cmdCurr->val) = 1; /* external so we turn the E bit */
                insertExt(sym, i); /* insert the external to extern list for later use */
            }
            if (!functionBreak && *sym == '&' && !strcmp(curr->name, sym + 1)) { /* relative address and match */
                setVal(cmdCurr->val, (curr->value) - i + 1); /* insert the gap value to the word */
                clearARE(cmdCurr->val); /* move all the bits 3 to the left */
                *(cmdCurr->val) |= makeMask(A_BIT); /* turn on the A(absolute) bit */
            }
            curr = curr->next; /* next symbol */
        }
    }
}

/**
 * Function: createFiles
 * Function Description: create object, entry, external files by given name 
 * 
 * @param fileName the file name
 */
void createFiles(char *fileName){
    createObject(fileName); /* create object file */
    createEntries(fileName); /* create entery file */
    createExtern(fileName); /* create extern file */
}

/**
 * Function: getDataTail
 * Function Description: gets the tail of the data list
 * 
 * @return the tail of data list
 */
dataNode * getDataTail(){
    dataNode * temp = dataHead;

    if(temp && temp->next){
        while (temp->next) {
            temp = temp->next;
        }
    }

    return temp;
}

/**
 * Function: getSymbolTail
 * Function Description: gets the tail of the symbol list
 * 
 * @return the tail of symbol list
 */
symbolNode * getSymbolTail(){
    symbolNode * temp = symHead;

    if(temp && temp->next){
        while (temp->next) {
            temp = temp->next;
        }
    }

    return temp;
}

/**
 * Function: getCmdTail
 * Function Description: gets the tail of the instruction list
 * 
 * @return the tail of instruction list
 */
cmdNode * getCmdTail(){
    cmdNode * temp = cmdHead;

    if(temp && temp->next){
        while (temp->next) {
            temp = temp->next;
        }
    }

    return temp;
}

/**
 * Function: getExtTail
 * Function Description: gets the tail of the external list
 * 
 * @return the tail of external list
 */
externNode * getExtTail(){
    externNode * temp = extHead;

    if(temp && temp->next){
        while (temp->next) {
            temp = temp->next;
        }
    }

    return temp;
}

/**
 * Function: createObject
 * Function Description: create object file that contain all the addresses and their hexadecimal value 
 * 
 * @param fName the file name
 */
void createObject(char *fName){
    FILE *fp;
    int i;
    unsigned int fixVal = 0;
    char fileName[FILE_NAME_LEN]="";
    dataNode *dataCurr; /* pointer to the tail of the data list */
    cmdNode *cmdCurr;

    dataCurr = getDataTail();
    cmdCurr = getCmdTail();

    strcpy(fileName, fName); /* insert the file name */
    strcat(fileName, ".ob"); /* add the suffix of object file */
    FILE_OPEN(fp, fileName,"w") /* open the current file with the given style and sets the pointer */
    fprintf(fp,"     %d %d\n", IC ,DC); /* print the number of instruction and number of data */
    for(i=0; i < IC && cmdCurr; i++){ /* run on instructions */
        fixVal = setInt(cmdCurr->val);
        fprintf(fp, "%07d %06x\n", i + IC_START, fixVal); /* print address and hexa */
        cmdCurr = cmdCurr->prev;
    }
    for(i=0;i<DC && dataCurr;i++){ /* run on data */
        fixVal = setInt(dataCurr->val);
        fprintf(fp, "%07d %06x\n", IC + IC_START + i, fixVal); /* print address and hexa */
        dataCurr = dataCurr->prev; /* prev node */
    }
    fclose(fp);
}

/**
 * Function: createEntries
 * Function Description: creates entry file that contain all the entries symbols and their addresses
 * 
 * @param fName the file name
 */
void createEntries(char *fName){
    FILE *fp;
    char fileName[FILE_NAME_LEN] = "";
    symbolNode *curr = getSymbolTail(); /* pointer to symbol-table tail */

    strcpy(fileName, fName); /* insert the file name */
    strcat(fileName, ".ent"); /* add the suffix .ent */
    FILE_OPEN(fp, fileName, "w") /* open the current file with the given style and sets the pointer */
    while(curr){ /* print the entrys from symbol table */
        if(curr->type & SYMBOL_ENTRY)
            fprintf(fp, "%s %07ld\n", curr->name, curr->value + IC_START); /* name and address */
        curr = curr->prev; /* prev node */
    }
    fclose(fp);
}

/**
 * Function: createExtern
 * Function Description: create extern file that contain all the extern symbols and their addresses
 * 
 * @param fName the file name
 */
void createExtern(char *fName){
    FILE *fp;
    char fileName[FILE_NAME_LEN] = "";
    externNode *curr = getExtTail(); /* pointer to external list tail */

    strcpy(fileName, fName); /* insert file name */
    strcat(fileName, ".ext"); /* add the suffix .ext */
    FILE_OPEN(fp, fileName, "w") /* open the current file with the given style and sets the pointer */
    while(curr){ /* run on extern list */
        fprintf(fp, "%s %07ld\n", curr->sym, curr->address + IC_START); /* name and address */
        curr = curr->prev; /* prev node */
    }
    fclose(fp);
}

/**
 * Function: freeAll
 * Function Description: free all the allocated memory in the program
 */
void freeAll(){
    freeSymbolList();
    freeDataList();
    freeCmdList();
    freeExternList();
    freeFixList();
}

/**
 * Function: freeSymbolList
 * Function Description: free all the allocated memory in the symbol list
 */
void freeSymbolList(){
    symbolNode *temp;
    
    while(symHead){ /* until the node head is null */
        temp = symHead->next; /* saves the next node */
        free(symHead);
        symHead = temp; /* set the head */
    }
}

/**
 * Function: freeDataList
 * Function Description: free all the allocated memory in the data list
 */
void freeDataList(){
    dataNode *temp;

    while(dataHead){ /* until the node head is null */
        temp = dataHead->next; /* saves the next node */
        free(dataHead->val);
        free(dataHead);
        dataHead = temp; /* set the head */
    }
}

/**
 * Function: freeCmdList
 * Function Description: free all the allocated memory in the instruction list
 */
void freeCmdList(){
    cmdNode *temp;

    while(cmdHead){ /* until the node head is null */
        temp = cmdHead->next; /* saves the next node */
        free(cmdHead->val); /* free the bitfield */
        free(cmdHead);
        cmdHead = temp; /* set the head */
    }
}

/**
 * Function: freeExternList
 * Function Description: free all the allocated memory in the external list
 */
void freeExternList(){
    externNode *temp;

    while(extHead){ /* until the node head is null */
        temp = extHead->next; /* saves the next node */
        free(extHead);
        extHead = temp; /* set the head */
    }
}

/**
 * Function: freeFixList
 * Function Description: free all the allocated memory in the fixe list
 */
void freeFixList(){
    fixNode *temp;

    while(fixHead){ /* until the node head is null */
        temp = fixHead->next; /* saves the next node */
        free(fixHead);
        fixHead = temp; /* set the head */
    }
}
