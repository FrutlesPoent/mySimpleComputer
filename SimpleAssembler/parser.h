#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

typedef struct _Command{
    int memoryNumber;
    int command;
    int operand;
} Command;

int readLine(FILE* fd, char* buff);
int chooseCommand(char* command);
int getName(char* str, char* name);
int getNumber(char* str, int* num);

Command* parse(char* str);
Command* createCommand(int memoryNumber, int command, int operand);
int sc_commandEncode(int command, int operand, int* value);




#endif