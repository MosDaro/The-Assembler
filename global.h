#ifndef ASSEMBLER_GLOBAL_H
#define ASSEMBLER_GLOBAL_H

#define FILE_NAME_LEN 30
#define MSG_LEN 100

typedef struct {
    char fileName[FILE_NAME_LEN];
    int lineNumber;
    char msg[MSG_LEN];
    int isHasError;
} fileData;

#endif