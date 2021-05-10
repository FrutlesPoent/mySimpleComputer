#ifndef PARSE_H
#define PARSE_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct _Command
{
    int number;
    int command;
    int operand;
} Command;

int readLine(FILE *fd, char *buff);
Command *parse(char *str);
int commandEncode(int command, int operand, int *value);
#endif