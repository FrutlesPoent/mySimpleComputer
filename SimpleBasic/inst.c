#include "inst.h"
#include "tree.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static char findVar(char* str) {
    char var = '\0';
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ')
            continue;
        if (var != '\0') {
            var = '\0';
            break;
        }
        if (isupper(str[i])) {
            var = str[i];
        } else {
            var = '\0';
        }
    }
    return var;
}

int chooseInstruction(char* instr, char* operand){
    if (!strcmp(instr, "INPUT")) {
        return inputInstruction(operand);
    } else if (!strcmp(instr, "PRINT")) {
        return printInstruction(operand);
    } else if (!strcmp(instr, "LET")) {
        return letInstruction(operand);
    } else if (!strcmp(instr, "IF")) {
        return ifInstruction(operand);
    } else if (!strcmp(instr, "GOTO")) {
        return gotoInstruction(operand);
    } else if (!strcmp(instr, "END")) {
        return endInstruction(operand);
    } else if (!strcmp(instr, "REM")) {
        return 0;
    }
    return -1;
}

int inputInstruction(char* inst) {
    char variable;
    if (isupper(inst[0])) // if it's symbol then it's command
        variable = inst[0];
    else
        return -1;
    addVariable(variable); // add variable to the instruction list where located variables
    addInstructionFirst("READ", variable); // add instruction
    return 0;
}

int printInstruction(char* inst) {
    char variable;
    if (isupper(inst[0])) // if it's symbol then it's command
        variable = inst[0];
    else
        return -1;
    addVariable(variable); // add variable to the instruction list where located variables
    addInstructionFirst("WRITE", variable); // add instruction
    return 0;
}

int letInstruction(char* inst) {
    int lenght = strlen(inst);
    char lVal = '\0';
    int i;
    for (i = 0; i < lenght; i++)
    {
        if (inst[i] == ' ') // if we hit space then continue
            continue;
        if (isupper(inst[i])) // check is it a symbol
        {
            lVal = inst[i]; // value = symbol of instruction
            break;
        }
        else if (inst[i] == '=') // if it's operator then it's no a instructions
        {
            return -1;
        }
    }
    int flag = 0;
    for (; i < lenght; i++) // read while not find '='
    {
        if (inst[i] == ' ')
            continue;
        if (inst[i] == '=')
        {
            flag = 1;
            break;
        }
    }
    if (!flag) // if we didn't get '=' then it's wrong line
        return -2;
    char rVal = evals(inst + i + 1); // result of right side of let
    if (!variableInAccum(rVal)) {
        moveAccum(rVal);
    }
    addVariable(lVal);
    storeAccum(lVal);
    // addInstructionFirst("STORE", lVal);
    return 0;
}

int ifInstruction(char* operand) {
    int lenght = strlen(operand);
    char lVal = '\0';
    int i;
    for (i = 0; i < lenght; i++)
    {
        if (operand[i] == ' ')
            continue;
        if (isdigit(operand[i])) {
            char buff[8];
            int index = 0;
            for (; i < lenght; i++){
                if (isdigit(operand[i]))
                    buff[index] = operand[i];
                else
                    break;
            }
            buff[index] = '\0';
            int val = atoi(buff);
            lVal = getLitName(val);
            break;
        }
        else if (isupper(operand[i]))
        {
            lVal = operand[i];
            break;
        }
        else if (operand[i] == '=')
        {
            return -1;
        }
    }
    int flag = 0;
    for (; i < lenght && flag == 0; i++) {
        if (operand[i] == ' ')
            continue;
        switch (operand[i]) {
            case '!': {
                if (operand[++i] == '=') // if !=
                    flag = 1;
                else
                    return -3;
                break;
            }
            case '<':
                flag = 2;
                break;
            case '>':
                flag = 3;
                break;
            case '=':
            if (operand[++i] == '=') // if '=='
                flag = 4;
            else
                return -3;
            break;
        }
    }
    if (!flag) // if not operation
        return -1;
    char buff[100];
    for (int j = 0; i < lenght; i++, j++){
        if (isupper(operand[i]) && isupper(operand[i + 1])) {
            buff[j] = '\0';
            break;
        }
        buff[j] = operand[i];
    }
    char instructionIf[50] = {'\0'};
    for (int j = 0; i < lenght; i++, j++) {
        if (isupper(operand[i])) {
            instructionIf[j] = operand[i];
        } else {
            instructionIf[j] = '\0';
            break;
        }
    }

    char rightValue = evals(buff);
    if (flag == 1){ // !=
        if(!variableInAccum(rightValue)) { // if variable is accum or  not
            moveAccum(rightValue); // move value to accum
        }
        addInstructionFirst("SUB", lVal);
        addInstructionThird("JZ", 2);
        chooseInstruction(instructionIf, &operand[i]);
    } else if (flag == 2) { // <
        if (!variableInAccum(rightValue)) { // if variable is accum or not
            moveAccum(rightValue); // move value to accum
        }
        addInstructionFirst("SUB", lVal);
        addInstructionThird("JNEG", 2);
        chooseInstruction(instructionIf, &operand[i]);
    } else if (flag == 3) { // >
        if (!variableInAccum(lVal)) {
            moveAccum(lVal);
        }
        addInstructionFirst("SUB", rightValue);
        addInstructionThird("JNEG", 2);
        chooseInstruction(instructionIf, &operand[i]);
    } else if (flag == 4) { // ==
        if (!variableInAccum(rightValue)) {
            moveAccum(rightValue);
        }
        addInstructionFirst("SUB", lVal);
        addInstructionThird("JNEG", 5);
        if (!variableInAccum(lVal)) {
            moveAccum(lVal);
        }
        addInstructionFirst("SUB", rightValue);
        addInstructionThird("JNEG", 2);
        chooseInstruction(instructionIf, &operand[i]);
    }
    return 0;
}

int gotoInstruction(char* inst) {
    char stringNumber[4];
    int stringLenght = strlen(inst);
    int j = 0;
    for (int i = 0; i < stringLenght; i++) {
        if (inst[i] == ' ')
            continue;
        if (isdigit(inst[i])){
            stringNumber[j] = inst[i];
            j++;
        }
    }
    stringNumber[j] = '\0';
    int string = atoi(stringNumber);
    int location = instructionLocationByString(string);
    if (location == -1) {
        addInstructionPromise("JUMP", string);
    } else {
        addInstructionSecond("JUMP", location);
    }
    // addInstructionSecond("JUMP", location);
    return 0;
}

int endInstruction(){
    addInstructionEnd();
    return 0;
}