#include <stdio.h>

#ifndef REGISTER_H
#define REGISTER_H

int sc_regInit();
int sc_regSet(int register, int value);
int sc_regGet(int register, int* value);
int sc_commandEncode(int command, int operand, int* value);
int sc_commandDecode(int value, int* command, int* operand);
void sc_printReg();

#endif