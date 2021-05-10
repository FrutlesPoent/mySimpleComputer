#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdio.h>
#include "string.h"

#define SIZE 100

typedef struct _Variable {
    char varName;
    int location;
    int isAccum;
    int isLiterall;
} Variable;


typedef struct _Inst{
    String* instruction;
    int line;
    int location;

} Inst;

typedef struct _Instruction {
    Variable** variables;
    Inst** instructions;
    int listSize;
    int listCapacity;
    int instructionListSize;
    int instructionListCapacity;
    int instructionStack;
    int variableStack;
    char varName;
} Instruction;

void initInstruction();
int setString(int string);
int addVariable(char var);
Variable* findVarS(char var, char storeAccum);
Variable* newVar(char var);
Variable* newVarSec(char var, int location);
void addInstructionFirst(char* inst, char var);
void addInstructionSecond(char* inst, int operand);
void addInstructionThird(char* inst, int offset);
void addInstructionEnd();
void moveAccum(char var);
int variableInAccum(char var);
void storeAccum(char var);
int instructionLocationByString(int string);
void popVar();
int isVar(int var);
int pushVar();
void writeInstruction(FILE* fd);

#endif