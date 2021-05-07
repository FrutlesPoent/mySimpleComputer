#include "cpu.h"

int removeValue(int value){
    int16_t buff = value;
    int16_t tmp = buff << 1;
    if ((tmp) < 0){
        buff = tmp >> 1;
    }
    return buff;
}

void inputMemorySec(int addres){
    printf("Input number or command\n");
    rk_mytermregime(1, 0, 0, 1, 1);
    scanf("%d", &typeOfCommand[addres]);
    int command, result, operand;
    if (!typeOfCommand[addres]){
        scanf("%2d%2d", &command, &operand);
        int retval = sc_commandEncode(command, operand, &result);
        if (retval != 0)
            printf("Input error\n");
        else
            sc_memorySet((operand), result);
    } else {
        scanf("%4d", &result);
        sc_memorySet((operand), result);
    }

    rk_mytermregime(0, 0, 0, 0, 1);
}

void loadAccum(int operand){
    int value;
    sc_memoryGet(operand, &value);
    sc_accum_set(value);
}

void memoryLoad(int operand){
    mt_gotoXY(1, 25);
    printf("Value\n");
    int value; 
    sc_memoryGet(operand, &value);
    printf("%04X", value);
    paintCell();
}

void load(int operand){
    if (operand >= SIZE)
        return -1;
    loadAccum(operand);
}

void restore(int operand){
    int value = sc_accum_get();
    sc_memorySet(operand, value);
}

int jmp(int operand){
    if (operand >= SIZE)
        return -1;
    if (operand <= 0)
        operand = 0;
    else 
        operand -= 1;
    sc_counter_set(operand);
    return 0;
}

int jneg(int operand){
    int value = removeValue(sc_accum_get());
    if (value < 0){
        operand -= 1;
        sc_counter_set(operand);
    }
    return 0;
}

int jz(int operand){
    int value = removeValue(sc_accum_get());
    if((value) == 0){
        operand -= 1;
        sc_counter_set(operand);
    }
    return 0;
}

int halt(int a){
    (void)a;
    raise(SIGUSR1);
    return 0;
}

int jc(int operand){
    int value = sc_accum_get();
    int val1;
    if (operand >= SIZE)
        return -1;
    sc_memoryGet(operand, &val1);
    if (val1 + value > 9999){
        sc_counter_set(operand);
    }
    return 0;
}

int alu(int command, int operand) {
    int value = sc_accum_get();
    int val1;

    switch(command){
        case 30:
            if (operand >= SIZE)
                return -1;
            sc_memoryGet(operand, &val1);
            value += val1;
            sc_accum_set(value);
            return 0;
        case 31:
            if (operand >= SIZE)
                return -1;
            sc_memoryGet(operand, &val1);
            value -= val1;
            sc_accum_set(value);
            return 0;
        case 32:
            if (operand >= SIZE)
                return -1;
            sc_memoryGet(operand, &val1);
            value /= val1;
            sc_accum_set(value);
            return 0;
        case 33:
            if (operand >= SIZE)
                return -1;
            sc_memoryGet(operand, &val1);
            value *= val1;
            sc_accum_set(value);
            return 0;

    }

    return -1;
}

int cu(){
    int value;
    int command;
    int operand;
    
    sc_memoryGet(sc_counter_get(), &value);
    sc_commandDecode(value, &command, &operand);
    if ((!command) || (!operand)){
        sc_regSet(IMP, 1);
        sc_regSet(COMMAND, 1);
        return -1;
    }

    if ((command > 29 && command < 34)){
        alu(command, operand);
    } else {
        switch (command) {
            case 10:
            if (operand >= SIZE)
                return - 1;
            inputMemorySec(operand);
            return 0;
        case 11:
            if (operand >= SIZE)
                return - 1;
            memoryLoad(operand);
            return 0;
        case 20:
            load(operand);
            return 0;
        case 21:
            restore(operand);
            return 0;
        case 40:
            jmp(operand);
            return 0;
        case 41:
            jneg(operand);
            return 0;
        case 42:
            jz(operand);
            return 0;
        case 43:
            halt(operand);
            return 0;
        case 56:
            jc(operand);
            return 0;
        }
        return -1;
    }

}