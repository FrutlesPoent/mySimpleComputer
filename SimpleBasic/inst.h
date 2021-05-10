#ifndef INST_H
#define INST_H

#include "instruction.h"
#include "string.h"

int chooseInstruction(char* instr, char* operand);
int inputInstruction(char* inst);
int printInstruction(char* inst);
int gotoInstruction(char* inst);
int letInstruction(char* inst);
int ifInstruction(char* inst);
int endInstruction();




#endif