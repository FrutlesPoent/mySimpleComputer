#include <stdio.h>

#ifndef STAT_H
#define STAT_H

#define MIN 0
#define SIZE 100

#define OVERFLOW 0b00000001 // overflow occured in action
#define DELZERO 0b00000010 // division by 0
#define OUTMEM 0b00000100 // out of memory
#define IMP 0b00001000 // ignore impulse
#define COMMAND 0b00010000 // wrong command

int memory[SIZE];

int flag;

#endif
