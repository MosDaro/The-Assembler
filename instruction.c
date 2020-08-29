/* instruction.c contain all the instruction parse and the word-code handle */
#include "instruction.h"

char *cmdList[] = {"","mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr",
                  "red","prn","rts","stop"};
char *regList[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

/* The function cmdParse parse the instruction and their parameters */
void cmdParse(char *sym, char *pars, fileData * fd) {
    char *c = pars , cmd[CMD_LEN] = ""; /* pointer to pars, the instruction name */
    int i = 0, word = 0, type; 

    while(!isspace(*c) && *c != '\0'){ /* insert the cmd */
        cmd[i++] = *c++;
    }

    if(sym) /* if there is a symbol insert it */
        insertSymbol(sym, CODE); /* code type */

    type = checkCmd(cmd);
    switch (type){ /* check the instruction and find their type */
        case CMD_NOT_FOUND:
            setErrorData(fd, "Instruction not found");
            break;
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
            twoParsCheck(c, type, &word, fd); /* handle the parameters */
            break;
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case JSR:
        case RED:
        case PRN:
            oneParCheck(c, type, &word, fd); /* handle the parameters */
            break;
        case RTS:
        case STOP:
            if(noParCheck(c, fd)) {
                insertCmd(word, 0, setWordNoPars(&word, type), fd); /* handle the parameters */
            }
            break;
    }
}

void * setWordNoPars(int *word, int type){
    if(type == RTS){ /* sets word with bit 19, 20, 21, 2 ON */
        *word = makeMask(SECOND_BIT_OPCODE) | makeMask(THIRD_BIT_OPCODE);
        *word |= makeMask(FORTH_BIT_OPCODE) | makeMask(A_BIT);
    }
    else if(type == STOP){ /* sets word with bit 18, 19, 20, 21, 2 ON */
        *word = makeMask(FIRST_BIT_OPCODE) | makeMask(SECOND_BIT_OPCODE);
        *word |= makeMask(THIRD_BIT_OPCODE) | makeMask(FORTH_BIT_OPCODE) | makeMask(A_BIT);
    }
    return NULL;
}

/* The function checkCmd search instruction number from cmdList and return the number, if not found return 0 */
int checkCmd(char *cmd){
    int i;
    int found = 0;

    for(i = 1 ;i < NUM_OF_CMD && !found; i++){ /* runs on instructions names */
        if(!strcmp(cmd, cmdList[i])){
            found = i; /* found match */
        }
    }

    return found;
}

int isValidSpecialCharsBase(char c, fileData * fd){
    int isValid = false;

    if(isdigit(c) || isalpha(c) || (c == '&' || c == '#' || c == '+' || c == '-')){
        isValid = true;
    }else {
        setErrorData(fd, "Special character is not valid in parameters");
    }

    return isValid;
}

int getParam(char *params, char * save, fileData * fd){
    char *c = params;
    char temp[LINE_OVER_MAX_SIZE] = "";
    int i = 0;

    BLANKJMP(c) /* ignore blanks */
    if(!strlen(c)){ /* no length no params */
        setErrorData(fd, "Missing parameters for instruction");
    }else{
        while (*c && !isspace(*c) && *c != ',' && isValidSpecialCharsBase(*c, fd)) { /* insert the first parameter */
            temp[i++] = *c;
            c++;
        }
        if(!checkSymbolLen(temp, fd)){
            save[0] = '\0';
        }
        if(!fd->isHasError) {
            temp[i] = '\0'; /* end of string */
            if (strlen(temp)) {
                strcpy(save, temp);
            } else {
                save[0] = '\0';
            }
        }
    }

    return c - params;
}

/* The function twoParsCheck handle instruction with two parameters and checks if the parameters is valid */
void twoParsCheck(char *pars, int type, int *word, fileData * fd) {
    char *c;
    char par1[LINE_OVER_MAX_SIZE]="", par2[LINE_OVER_MAX_SIZE]=""; /* parameters names to save */
    int i;

    setWordTwoPars(word, type); /* handles the word */
    i = getParam(pars, par1, fd);
    if(!fd->isHasError){
        c = pars + i;
        BLANKJMP(c) /* ignore blanks */
        if(!strlen(c)){
            setErrorData(fd, "Operation requires 2 operands");
        } else if(*c != ','){ /* no comma */
            setErrorData(fd, "Must be comma for separate between params");
        } else {
            c++; /* ignore , */
            if(!strlen(c)) { /* missing second parameter */
                setErrorData(fd, "Missing second parameter for instruction");
            }
            if(!fd->isHasError) {
                i = c - pars;
                i += getParam(c, par2, fd);
                if(!fd->isHasError && strlen(pars + i)) {
                    setErrorData(fd, "Extra letters after the parameters");
                }
            }
        }
    }

    if(!fd->isHasError) {
        handleAddressingTwoParams(type, par1, par2, word, fd);
    }
}

void setWordTwoPars(int *word, int type){

    switch (type){
    case MOV: /* no manipulation on word */
        break;
    case CMP: /* sets word with bit 18 ON */
        *word = makeMask(FIRST_BIT_OPCODE);
        break;
    case ADD: /* sets word with bit 19, 3 ON */
        *word = makeMask(SECOND_BIT_OPCODE) | makeMask(FIRST_FNCT); 
        break;
    case SUB: /* sets word with bit 19, 4 ON */
        *word = makeMask(SECOND_BIT_OPCODE) | makeMask(SECOND_FNCT);
        break;
    case LEA: /* sets word with bit 20 ON */
        *word = makeMask(THIRD_BIT_OPCODE);
        break;
    }
}

/* This function handles the groups that have the same addresing in the same parameters */
void handleAddressingTwoParams(int type, char *par1, char *par2, int *word, fileData * fd){
	switch (type) { /* handle by groups that have the same addressing in the same parameters */
		case MOV:
		case ADD:
		case SUB:
			Mov_Add_Sub_Pars(par1, par2, word, fd); /* addressing: par1: 0,1,3  par2: 1,3  */
			break;
		case CMP:
			Cmp_Pars(par1, par2, word, fd); /* addressing: par1: 0,1,3  par2: 0,1,3 */
			break;
		case LEA:
			Lea_Pars(par1, par2, word, fd); /* addressing: par1: 1  par2: 1,3 */
			break;
	}
}

int noParCheck(char * line, fileData * fd){
    char * c = line;

    BLANKJMP(c);
    if(strlen(c)){
        setErrorData(fd, "Operation requires no operands");
    }
    return !fd->isHasError;
}

/* The function oneParCheck handle instruction with one parameter and checks if the parameter is valid */
void oneParCheck(char *line, int type, int *word, fileData * fd) {
    char *c = line, par[LINE_OVER_MAX_SIZE]=""; /* pointer to line, to save the parameter */
    int i;

    setWordOnePar(word,type);
    BLANKJMP(c) /* ignore blanks */
    i = c - line;
    if(!strlen(c)) { /* no length no params */
        setErrorData(fd, "Missing parameter for instruction");
    }else{
        i += getParam(c, par, fd);
        if(!fd->isHasError) {
            c = line + i;
            BLANKJMP(c) /* ignore blanks */
            if(strlen(c)) { /* check for extra letters */
                setErrorData(fd, "Extra letters after the parameter");
            }else{
                handleAddressingOneParam(type, par, word, fd);
            }
        }
    }
}

void setWordOnePar(int *word, int type){

    switch (type){
        case CLR: /* sets word with bit 18, 20, 3 ON */
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(THIRD_BIT_OPCODE) | makeMask(FIRST_FNCT);
            break;
        case NOT: /* sets word with bit 18, 20, 4 ON */
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(THIRD_BIT_OPCODE) | makeMask(SECOND_FNCT);
            break;
        case INC: /* sets word with bit 18, 20, 3, 4 ON */
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(THIRD_BIT_OPCODE) | makeMask(FIRST_FNCT) | makeMask(SECOND_FNCT);
            break;
        case DEC: /* sets word with bit 18, 20, 5 ON */
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(THIRD_BIT_OPCODE) | makeMask(THIRD_FNCT);
            break;
        case JMP: /* sets word with bit 18, 21, 3 ON */
            *word = makeMask(FIRST_BIT_OPCODE) |  makeMask(FORTH_BIT_OPCODE) | makeMask(FIRST_FNCT);
            break;
        case BNE: /* sets word with bit 18, 21, 4 ON */
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(FORTH_BIT_OPCODE) | makeMask(SECOND_FNCT);
            break;
        case JSR: /* sets word with bit 18, 21, 3, 4 ON */ 
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(FORTH_BIT_OPCODE) | makeMask(SECOND_FNCT) | makeMask(FIRST_FNCT);
            break;
        case RED: /* sets word with bit 20, 21 ON */ 
            *word = makeMask(THIRD_BIT_OPCODE) | makeMask(FORTH_BIT_OPCODE);
            break;
        case PRN: /* sets word with bit 18, 20, 21 ON */ 
            *word = makeMask(FIRST_BIT_OPCODE) | makeMask(THIRD_BIT_OPCODE) | makeMask(FORTH_BIT_OPCODE);
            break;
    }
}

/* immediate addressing  check (method number 0)*/
int immediateAddress(char *par, fileData * fd){
    char *c = par; /* pointer to parameter */
    int res = 1;

    if(*c != '#') /* not immediate */
        res = 0;
    else {
        c++; /* jump above # */
        if (*c == '+' || *c == '-') /* jump above sign */
            c++;
        while (res != -1 && !isspace(*c) && *c != '\0') { /* not blank and not end of string */
            if (!isdigit(*c)) { /* not digit */
                setErrorData(fd, "The immediate addressing not number");
                res = -1; /*invalid */
            }
            c++;
        }
        if(!fd->isHasError) {
            BLANKJMP(c) /* ignore blanks */
            if (strlen(c)) { /* left at list one char */
                setErrorData(fd, "The parameter cant contain blanks");
                res = -1; /* invalid */
            }
        }
	}
	return res;   
}

/* The function handles the groups that have the same addresing parameter */
void handleAddressingOneParam(int type, char *par, int *word, fileData * fd){
    switch (type) { /* handle by groups that have the same addressing parameter */
		case CLR:
		case NOT:
		case INC:
		case DEC:
		case RED:
			Clr_Not_Inc_Dec_Red_Par(par, word, fd); /* addressing: par: 1,3 */
			break;
		case JMP:
		case BNE:
		case JSR:
			Jmp_Bne_Jsr_Par(par, word, fd); /* addressing: par: 1,2 */
			break;
		case PRN:
			Prn_Pars(par, word, fd); /* addressing: par: 0,1,3 */
			break;
	}
}


/* direct addressing check (method number 1) */
int directAddress(char *par, fileData * fd){
    char *c = par; /* pointer to parameter */
    int res = 1; /* passed the checks */

    if(!isalpha(*c)) /* not direct */
        res = 0;
    else {
        while (res != -1 && !isspace(*c) && *c != '\0') { /* not blank and not end of string  */
            if (!isalpha(*c) && !isdigit(*c)) { /* not alpha nor digit */
                setErrorData(fd, "The direct addressing must contain only letter or numbers");
                res = -1;
            }
            c++;
        }
        if(res != -1) {
            BLANKJMP(c) /* ignore blanks */
            if (strlen(c)) { /* left at list one char */
                setErrorData(fd, "The parameter cant contain blanks");
                res = -1; /* invalid */
            }
        }
	}
	return res;
}

/* relative addressing check (method number 2) */
int relativeAddress(char *par, fileData * fd) {
    char *c = par; /* pointer to parameter */
    int res = 1;
    
    if(*c != '&') /* not relative */
        res = 0;
    else {
        c++; /* jump above & */
        if (!isalpha(*c)) { /* second char */
            setErrorData(fd, "Invalid relative address");
            res = -1; /* invalid */
        }else {
            while (res != -1 && !isspace(*c) && *c != '\0') { /* not blank and not end of string */
                if (!isalpha(*c) && !isdigit(*c)) { /* not alpha nor digit */
                    setErrorData(fd, "The relative addressing not a symbol");
                    res = -1; /* invalid */
                }
                c++;
            }
            if(!fd->isHasError) {
                BLANKJMP(c) /* ignore blanks */
                if (strlen(c)) { /* left at list one letter */
                    setErrorData(fd, "The parameter cant contain blanks");
                    res = -1; /* invalid */
                }
            }
        }
	}
	return res;
}

/* register addressing check (method number 3) */
int registerAddress(char *par, fileData * fd) {
    int i;
    int isFoundMatch = -1;

    if(*par != 'r')
        isFoundMatch = 0; /* not register */
    else {
        for (i = 0; i < NUM_OF_REG && isFoundMatch != 1; i++) /* search for the match register */
            if (!strcmp(par, regList[i]))
                isFoundMatch = 1; /* found match */
    }
    if(isFoundMatch == -1) {
        setErrorData(fd, "Invalid register");
    }
    return isFoundMatch;
}

void handleRegister(int *word, int *p, char *par, int isSource){
    if(isSource){
        *word |= makeMask(FIRST_ADR_SOUR) | makeMask(SECOND_ADR_SOUR) | (atol(par+1) << FIRST_REG_SOUR);
    }
    else{
        *word |= makeMask(FIRST_ADR_DEST) | makeMask(SECOND_ADR_DEST) | (atol(par+1) << FIRST_REG_DEST);
        
    }
    *word |= makeMask(A_BIT);
    *p = true; /* param done, flag on */
}

void handleImmidiate(int *word, int *wordPar, int *flag, int *p, char *par){
    *word |= makeMask(A_BIT); /* set word with bit 2 on */
    *wordPar = (atol(par+1) << FIRST_FNCT) | makeMask(A_BIT); /* set the immediate value, move to the right spot and turn on the bit #2 */
    *flag = true; /* first immediate value known, flag on */
    *p = true; /* first param done, flag on */
}

void handleDirect(int *word, int *wordPar, int isSource){
    if(isSource){
        *word |= makeMask(FIRST_ADR_SOUR); /* turn on the bits #16, #2 */
    }
    else{
        *word |= makeMask(FIRST_ADR_DEST); /* turn on bits #11, #2 */
    }
    *word |= makeMask(A_BIT);
    if(wordPar)
        *wordPar = true; /* param defined */
}

/* hanndle the parameters of instructions mov, add and sub, decide which param is what type and set the word */
void Mov_Add_Sub_Pars(char *par1, char *par2, int *word, fileData * fd){
    int p1 = 0, p2 = 0; /* first and second params flags */
    int word1 = 0, word2 = 0; /* first and second params words flags */
    int flag1 = 0, flag2 = 0; /* flags to known word value */

    if(relativeAddress(par1, fd) == 1 || relativeAddress(par2, fd) == 1) {/* not relative check */
        setErrorData(fd, "This instruction can't contain relative parameters");
    }
    if(!fd->isHasError && immediateAddress(par2, fd) == 1) { /* second par not immediate */
        setErrorData(fd, "In this instruction the second parameter can't be immediate");
    }
    if(!fd->isHasError && registerAddress(par1, fd) == 1){ /* if first param register handle him */
        /* sets the word with bit 16, 17 ON, turns A bit, register number moves 13 bits left */
        handleRegister(word, &p1, par1, true); /* first param done, flag on */
    }
    if(!fd->isHasError &&  !p1 && immediateAddress(par1, fd) == 1) { /* if first param not handled check if he is immediate */
        handleImmidiate(word, &word1, &flag1, &p1, par1); /* first param done */
    }
    if(!fd->isHasError && !p1 && directAddress(par1, fd) == 1) { /* if param 1 is direct */
        handleDirect(word, &word1, true); /* param1 defined */
    }
    if(!fd->isHasError) {
        if (registerAddress(par2, fd) == 1) { /* second param register */
            /* turn on bits #11, 12#, move register number to the spot and turn on bit #2, second param immediate, bit 2 on */
            handleRegister(word, &p2, par2, false); /* parameter 2 is done */
        } else if (!p2 && immediateAddress(par2, fd) == 1) { /* second param immediate */
            handleImmidiate(word, &word2, &flag2, &p2, par2); /* parameter 2 done */
        } else if (!p2 && directAddress(par2, fd) == 1) { /* second param direct */
            handleDirect(word, &word2, false); /* param2 defined */
        }
    }
    if(!fd->isHasError) {
        insertCmd(*word, 0, NULL, fd); /* insert to instruction list */
        if (!fd->isHasError && word1) { /* if first param was set */
            /* if known value insert the param value else insert the param value as zero */
            (flag1) ? insertCmd(word1, 0, NULL, fd) : insertCmd(0, IC, par1, fd);
        }
        if (!fd->isHasError && word2) { /* if second param was set */
            /* if known value insert the param value else insert the param value as zero */
            (flag2) ? insertCmd(word2, 0, NULL, fd) : insertCmd(0, IC, par2, fd);
        }
    }
}

/* hanndle the parameters of instruction cmp, decide which param is what type and set the word */
void Cmp_Pars(char *par1, char *par2, int *word, fileData * fd) {
    int p1 = 0, p2 = 0; /* first and second parameters seted flag */
    int word1 = 0, word2 = 0; /* first and second parameter defined flag */
    int flag1 = 0, flag2 = 0; /* known value flags */

    if(relativeAddress(par1, fd) == 1 || relativeAddress(par2, fd) == 1) { /* cant be relatives */
        setErrorData(fd, "In the instruction cmp the parameters can't be relative address");
    }
    if(!fd->isHasError && registerAddress(par1, fd) == 1) { /* first param is register */
        /* turn on bits #16, #17, insert the register num to the spot and turn bit #2 on */
        handleRegister(word, &p1, par1, true); /* param 1 is done */
    } else if(!fd->isHasError && !p1 && immediateAddress(par1, fd) == 1) { /* first parameter is immidiate */
        /* bit 2 on, insert the immidiate value to the spot and turn on bit #2 */
        handleImmidiate(word, &word1, &flag1, &p1, par1); /* first param is set */
    } else if(!fd->isHasError && !p1 && directAddress(par1, fd) == 1) {/* first param is direct */
        handleDirect(word, &word1, true); /* param1 defined */
    }
    if(!fd->isHasError && registerAddress(par2, fd) == 1) { /* second param is register */
        /* turn on bit #11, #12 insert the number of register to the spot and turn bit #2 on */
        handleRegister(word, &p2, par2, false); /* second param is done */ 
    } else if(!fd->isHasError && !p2 && immediateAddress(par2, fd) == 1) { /* second param is immidiate */
        /* bit 2 on, insert the immidiate value to the spot and turn bit #2 on */
        handleImmidiate(word, &word2, &flag2, &p2, par2); /* second param done */
    } else if(!fd->isHasError && !p2 && directAddress(par2, fd) == 1) { /* second param is direct */
        handleDirect(word, &word2, false); /* param2 defined */
    }
    if(!fd->isHasError) {
        insertCmd(*word, 0, NULL, fd); /* insert the instruction word */
        if (word1) { /* if first param defined */
            /* if known value insert first param value to list else insert first param value set to zero */
            (flag1) ? insertCmd(word1, 0, NULL, fd) : insertCmd(0, IC, par1, fd);
        }
        if (word2) { /* if second param defined */
            /* if knwon value insert second param value to list else insert second param value set to zero */
            (flag2) ? insertCmd(word2, 0, NULL, fd) : insertCmd(0, IC, par2, fd);
        }
    }
}

/* handle the parameters of instruction lea, decide which param is what type and set the word */
void Lea_Pars(char *par1, char *par2, int *word, fileData * fd) {
    int p2 = 0; /* second param flag done */
    int word1 =0, word2 = 0; /* first and second params defined */

    /* check if first param not immediate not relative and not register */
    if(immediateAddress(par1, fd) == 1 || relativeAddress(par1, fd) == 1 || registerAddress(par1, fd) == 1) {
        setErrorData(fd, "In the instruction lea the first parameter can be only direct address");
    }
    if(!fd->isHasError && directAddress(par1, fd) == 1) { /* first param direct */
        /* turn on bits #16, #2 */
        handleDirect(word, &word1, true); /* param1 defined */
    }
    if(!fd->isHasError && (immediateAddress(par2, fd) == 1 || relativeAddress(par2, fd) == 1)) { /* second param not immidate nor relative */
        setErrorData(fd, "The second parameter can be only direct or register address");
    }
    if(!fd->isHasError && registerAddress(par2, fd) == 1) { /* second param register */
        /* turns on bits #11, #12 insert register number to the spot and turn on bit #2 */
        handleRegister(word, &p2, par2, false); /* second param is done */
    } else if(!fd->isHasError && !p2 && directAddress(par2, fd) == 1) { /* second param is direct */
        /* turns on bits #11, #2 */
        handleDirect(word, &word2, false);
    }
    if(!fd->isHasError) {
        insertCmd(*word, 0, NULL, fd); /* insert instruction value */
        if (word1) /* first param defined */
            insertCmd(0, IC, par1, fd); /* insert first param set to zero */
        if (word2) /* second param defined */
            insertCmd(0, IC, par2, fd); /* insert second param set to zero */
    }
}

/* 
* hanndle the parameters of instructions clr, not, inc, dec, red and par
* decide which param is what type and set the word 
*/
void Clr_Not_Inc_Dec_Red_Par(char *par, int *word, fileData * fd) {
    int word1 = 0; /* param defined flag */
    int p1 = 0; /* param done flag */

    if(immediateAddress(par, fd) == 1 || relativeAddress(par, fd) == 1) {/* check if param not immidiate nor relative */
        setErrorData(fd, "In this instruction the parameter can be only directive or register address");
    }
    if(!fd->isHasError && registerAddress(par, fd) == 1) { /* param is register */
        /* turns on bits #11, #12, #2 insert number of register to the right spot */
        handleRegister(word, &p1, par, false); /* param is done */
    } else if(!fd->isHasError && !p1 && directAddress(par, fd) == 1) { /* parameter  is direct */
        /* turn on bits #11, #2 */
        handleDirect(word, &word1, false); /* parameter defined */
    }
    if(!fd->isHasError) {
        insertCmd(*word, 0, NULL, fd); /* insert instruction word */
        if (word1) /* if defined insert it zero value */
            insertCmd(0, IC, par, fd);
    }
}

/* handle the parameter of instructions jmp,bne,jsr decide which param is what type and set the word */
void Jmp_Bne_Jsr_Par(char *par, int *word, fileData * fd) {
    int p1 = 0; /* flag for parameter done */

    if(immediateAddress(par, fd) == 1 || registerAddress(par, fd) == 1) { /* check if not immediate nor register */
        setErrorData(fd, "In this instruction the parameter can be only directive or relative address");
    }
    if(!fd->isHasError && directAddress(par, fd) == 1) { /* direct */
        /* turns on bits #11, #2 */
        handleDirect(word, NULL, false); /* parameter done */
    } else if(!fd->isHasError && !p1 && relativeAddress(par, fd) == 1) { /* relative */
        *word |= makeMask(SECOND_ADR_DEST) | makeMask(A_BIT); /* turns on bits #12, #2 */
    }
    if(!fd->isHasError) {
        insertCmd(*word, 0, NULL, fd); /* insert instruction word */
        insertCmd(0, IC, par, fd); /* insert param word set to zero */
    }
}


/* hanndle the parameters of instruction prn, decide which param is what type and set the word */
void Prn_Pars(char *par, int *word, fileData * fd) {
    int word1 = 0; /* flag param defined */
    int flag1 = 0; /* flag if knwon value */
    int p1 = 0; /* param done */

    if(relativeAddress(par, fd) == 1) { /* check if not relative */
        setErrorData(fd, "In the instruction prn the parameter can't be relative address");
    } else if(registerAddress(par, fd) == 1) { /* register */
        /* turns on bits #11, #12 insert register number to the spot and turn on bit #2 */
        handleRegister(word, &p1, par, false); /* param  done */
    }else if(!p1 && immediateAddress(par, fd) == 1) { /* immidiate */
        /* turns on bit #2 insert the immidaite value to the spot and turns on bit #2 */
        handleImmidiate(word, &word1, &flag1, &p1, par); /* param done */
    } else if(!p1 && directAddress(par, fd) == 1) { /* direct */
        /* turns on bits #11, 2 */
        handleDirect(word, &word1, false); /* param defined */
    }
    if(!fd->isHasError) {
        insertCmd(*word, 0, NULL, fd); /* insert the instruction word */
        if (word1) { /* param defined */
            /* if known value insert the value else insert the value set to zero */
            (flag1) ? insertCmd(word1, 0, NULL, fd) : insertCmd(0, IC, par, fd);
        }
    }
}