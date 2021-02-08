#include "memory.h"
#include "stat.h"
#include "register.h"
#include <stdio.h>


int sc_memoryInit(){
    for (int i = 0; i < SIZE; i++)
        memory[i] = 0;

    return 0;
}

int sc_memorySet(int address, int value){
    if (address > SIZE || address < MIN){
         sc_regSet(OUTMEM, 1); // out of memory
         return 1;
    }

    memory[address] = value;

    return 0;
}

int sc_memoryGet(int address, int* value){

    if (address > SIZE || address < MIN){
        sc_regSet(OUTMEM, 1); // out of memory
        return 1;
    }

    *value = memory[address];

    return 0;
}

int sc_memorySave(char* filename){

    FILE *file = fopen(filename,"wb");
    fwrite(memory, sizeof(int), SIZE, file);
    fclose(file);
    
    return 0;
}

int sc_memoryLoad(char* filename){

    FILE *file = fopen(filename, "rb");
    fread(memory, sizeof(int), SIZE, file);
    fclose(file);

    return 0;
}

