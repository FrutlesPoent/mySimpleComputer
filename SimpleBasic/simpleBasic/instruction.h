#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "parse.h"
#include "string.h"

int inputInstruction(char *operand);
int printInstruction(char *operand);
int letInstruction(char *operand);
int ifInstruction(char *operand);
int gotoInstruction(char *operand);
int haltInstruction();
int choiceInstruction(char *instruction, char *operand);

#endif