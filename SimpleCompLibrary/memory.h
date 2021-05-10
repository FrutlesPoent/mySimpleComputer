#include <stdio.h>
#include "stat.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#ifndef MEMORY_H
#define MEMORY_H

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int* value);
int sc_memorySave(char* filename);
int sc_memoryLoad(char* filename);


#endif