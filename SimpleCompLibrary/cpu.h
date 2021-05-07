#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>
#include "register.h"
#include "memory.h"
#include "stat.h"
#include "terminal.h"
#include <signal.h>
#include "readkey.h"
int typeOfCommand[100];

int alu(int command, int operand);
void cpu();
void memoryLoad(int operand);
// void inputMemory(int operand);
void load(int operand);
void restore(int operand);
int jmp(int operand);
int jnge(int operand);
int jz(int operand);
int halt(int operand);
// int jc(int operand);
// void inputMemory();
int removeValue(int value);
#endif