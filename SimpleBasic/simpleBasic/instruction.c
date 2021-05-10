#include "instruction.h"
#include <string.h>
#include <stdlib.h>
#include "tree.h"

int choiceInstruction(char *instruction, char *operand)
{

    if (!strcmp(instruction, "INPUT"))
    {
        return inputInstruction(operand);
    }
    else if (!strcmp(instruction, "PRINT"))
    {
        return printInstruction(operand);
    }
    else if (!strcmp(instruction, "LET"))
    {
        return letInstruction(operand);
    }
    else if (!strcmp(instruction, "IF"))
    {
        return ifInstruction(operand);
    }
    else if (!strcmp(instruction, "GOTO"))
    {
        return gotoInstruction(operand);
    }
    else if (!strcmp(instruction, "END"))
    {
        return haltInstruction();
    }
    else if (!strcmp(instruction, "REM"))
    {
        return 0;
    }
    return -1;
}

int inputInstruction(char *operand)
{
    char variable;
    if (isupper(operand[0]))
        variable = operand[0];
    else
        return -1;
    addVariable(variable);
    addInstructionVariable("READ", variable);
    return 0;
}
int printInstruction(char *operand)
{
    char variable;
    if (isupper(operand[0]))
        variable = operand[0];
    else
        return -1;
    addVariable(variable);
    addInstructionVariable("WRITE", variable);
    return 0;
}
int letInstruction(char *operand)
{
    int lenght = strlen(operand);
    char lVal = '\0';
    int i;
    for (i = 0; i < lenght; i++)
    {
        if (operand[i] == ' ')
            continue;
        if (isupper(operand[i]))
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
    for (; i < lenght; i++)
    {
        if (operand[i] == ' ')
            continue;
        if (operand[i] == '=')
        {
            flag = 1;
            break;
        }
    }
    if (!flag)
        return -2;
    char rVal = evals(operand + i + 1);
    if (!variableInAccumulator(rVal))
    {
        loadAccumulator(rVal);
    }
    addVariable(lVal);
    addInstructionVariable("STORE", lVal);
    return 0;
}
int ifInstruction(char *operand)
{
    int lenght = strlen(operand);
    char lVal = '\0';
    int i;
    for (i = 0; i < lenght; i++)
    {
        if (operand[i] == ' ')
            continue;
        if (isupper(operand[i]))
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
    for (; i < lenght && flag == 0; i++)
    {
        if (operand[i] == ' ')
            continue;
        switch (operand[i])
        {
        case '!':
        {
            if (operand[i + 1] == '=')
            {
                flag = 1;
                i++;
            }
            else
                return -1;
            break;
        }
        case '<':
            flag = 2;
            break;
        case '>':
            flag = 3;
            break;
        case '=':
            if (operand[i + 1] == '=')
            {
                flag = 4;
                i++;
            }
            else
                return -1;
            break;
        }
    }
    if (!flag)
        return -1;
    char buff[100];
    for (int j = 0; i < lenght; i++, j++)
    {
        if (isupper(operand[i]) && isupper(operand[i + 1]))
        {
            buff[j] = '\0';
            break;
        }
        buff[j] = operand[i];
    }
    char instructionIf[50] = {'\0'};
    for (int j = 0; i < lenght; i++, j++)
    {
        if (isupper(operand[i]))
        {
            instructionIf[j] = operand[i];
        }
        else
        {
            instructionIf[j] = '\0';
            break;
        }
    }

    char rVal = evals(buff);
    if (flag == 1)
    {
        if (!variableInAccumulator(rVal))
        {
            loadAccumulator(rVal);
        }
        addInstructionVariable("SUB", lVal);
        addInstructionj("JZ", 2);
        choiceInstruction(instructionIf, &operand[i]);
    }
    else if (flag == 2)
    {
        if (!variableInAccumulator(rVal))
        {
            loadAccumulator(rVal);
        }
        addInstructionVariable("SUB", lVal);
        addInstructionj("JNEG", 2);
        choiceInstruction(instructionIf, &operand[i]);
    }
    else if (flag == 3)
    {
        if (!variableInAccumulator(lVal))
        {
            loadAccumulator(lVal);
        }
        addInstructionVariable("SUB", rVal);
        addInstructionj("JNEG", 2);
        choiceInstruction(instructionIf, &operand[i]);
    }
    else if (flag == 4)
    {
        if (!variableInAccumulator(rVal))
        {
            loadAccumulator(rVal);
        }
        addInstructionVariable("SUB", lVal);
        addInstructionj("JNEG", 5);
        if (!variableInAccumulator(lVal))
        {
            loadAccumulator(lVal);
        }
        addInstructionVariable("SUB", rVal);
        addInstructionj("JNEG", 2);
        choiceInstruction(instructionIf, &operand[i]);
    }
    return 0;
}

int gotoInstruction(char *operand)
{
    char lineNumber[4];
    int lenght = strlen(operand);
    int j = 0;
    for (int i = 0; i < lenght; i++)
    {
        if (operand[i] == ' ')
            continue;
        if (!isdigit(operand[i]))
            return -1;
        lineNumber[j] = operand[i];
        j++;
    }
    lineNumber[j] = '\0';
    int line = atoi(lineNumber);
    int location = instructionLocationByLine(line);
    if (location == -1)
        return -1;
    addInstructionOperand("JUMP", location);
    return 0;
}
int haltInstruction()
{
    addInstructionHalt();
    return 0;
}