#include <stdio.h>

#ifndef REGISTER_H
#define REGISTER_H

static int accumReg = 0;
static int counterReg = 0;

int sc_regInit();
int sc_regSet(int register, int value);
int sc_regGet(int register, int* value);
int sc_commandEncode(int command, int operand, int* value);
int sc_commandDecode(int value, int* command, int* operand);
void sc_printReg();
void sc_accum_set(int value);
int sc_accum_get();
void sc_counter_set(int value);
int sc_counter_get();

#endif