#include <stdio.h>
#include "register.h"
#include "stat.h"


int sc_regInit(){
    flag = 0;

    return 0;
}

int sc_regSet(int sregister, int value) {

    if (sregister != OVERFLOW || sregister != DELZERO || sregister != OUTMEM || sregister != IMP ||
    sregister != COMMAND) {
        if (value = 0)
            flag = flag & ~ sregister;
        else if (value = 1)
            flag = flag | sregister;
        else
            return 0;

    }

    return 0;
}

int sc_regGet(int sregister, int* value){

    if (sregister != OVERFLOW || sregister != DELZERO || sregister != OUTMEM || sregister != IMP ||
    sregister != COMMAND) {

        if ((flag & sregister) == 0)
            *value = 0;
        else
            *value = 1;
    }

    return 0;
}

int sc_commandEncode(int command, int operand, int* value){

    if ((command < 10) || (command > 11 && command < 20) 
        || (command > 21 && command < 30)
        || (command > 33 && command < 40) 
        || (command > 43 && command < 51)
        || (command > 76)){
            sc_regSet(COMMAND, 1);
            return -1;
        }
    if (operand > 127 || operand < 0){
        return 1;
    }

    *value = (command << 7) | operand;

    return 0;

}

int sc_commandDecode(int value, int* command, int* operand){

    if ((value >> 14 ) != 0){
        sc_regSet(COMMAND, 1);
        return 1;
    }


    *command = value >> 7;
    *operand = value & 0x7F;

    return 0;

}
void sc_printReg(){
    printf("%d", flag);
}

void sc_accum_set(int value){
    accumReg = value;
}

int sc_accum_get(){
    return accumReg;
}

void sc_counter_set(int value){
    counterReg = value;
}

int sc_counter_get(){
    return counterReg;
}
