#ifndef PARSE_H
#define PARSE_H
#include "string.h"
#include <stdio.h>
#include <malloc.h>

#define ACCUM_LOCATION -2

void initContext();
int addVariable(char name);
int variableInAccumulator(char name);
int pushVariable();
void popVariable();
int isVariable(int name);
int instructionLocationByLine(int line);
void addInstructionVariable(char *instruction, char name);
void addInstructionOperand(char *instruction, int operand);
void addInstructionj(char *instruction, int offset);
void addInstructionHalt();
int setCurrentLine(int line);
void loadAccumulator(char name);
void storeAccumulator(char name);
void writeInstruction(FILE *fd);

#endif