#include "parse.h"

Command *newCommand(int number, int command, int operand)
{
    Command *com = malloc(sizeof(*com));
    if (com == NULL)
        return com;
    com->number = number;
    com->command = command;
    com->operand = operand;

    return com;
}

int readLine(FILE *file, char *buff)
{
    char symbol;
    int i = 0;
    while (1)
    {
        symbol = fgetc(file);
        if (symbol == EOF)
            break;
        if (symbol == '\n')
            break;
        if (symbol == ';')
        {
            while ((symbol = fgetc(file)) != '\n')
                continue;
            break;
        }
        buff[i++] = symbol;
    }
    buff[i] = '\0';
    return i;
}

int getNum(char *str, int *num)
{
    int i = 0;
    char buff[8];
    while (str[i] != ' ' && str[i] != '\0')
    {
        if (!isdigit(str[i]))
        {
            return -1;
        }
        buff[i] = str[i];
        i++;
    }
    if (i == 0)
        return -1;
    buff[i] = '\0';
    *num = atoi(buff);
    return i;
}

int getName(char *str, char *name)
{
    int i = 0;
    while (str[i] != ' ')
    {
        if (isdigit(str[i]) || str[i] == '\n')
            return -1;
        name[i] = str[i];
        i++;
    }
    if (i == 0)
        return -1;
    name[i] = '\0';
    return i;
}

int choiseCommand(char *command)
{
    if (!strcmp(command, "READ"))
    {
        return 10;
    }
    else if (!strcmp(command, "WRITE"))
    {
        return 11;
    }
    else if (!strcmp(command, "LOAD"))
    {
        return 20;
    }
    else if (!strcmp(command, "STORE"))
    {
        return 21;
    }
    else if (!strcmp(command, "ADD"))
    {
        return 30;
    }
    else if (!strcmp(command, "SUB"))
    {
        return 31;
    }
    else if (!strcmp(command, "DIVIDE"))
    {
        return 32;
    }
    else if (!strcmp(command, "MUL"))
    {
        return 33;
    }
    else if (!strcmp(command, "JUMP"))
    {
        return 40;
    }
    else if (!strcmp(command, "JNEG"))
    {
        return 41;
    }
    else if (!strcmp(command, "JZ"))
    {
        return 42;
    }
    else if (!strcmp(command, "HALT"))
    {
        return 43;
    }
    else if (!strcmp(command, "SUBC"))
    {
        return 66;
    }
    else if (!strcmp(command, "="))
    {
        return 0;
    }
    return -1;
}
Command *parse(char *str)
{
    int number, command, operand;
    int retval = getNum(str, &number);
    if (retval == -1)
    {
        printf("Неверный номер памяти\n");
        return NULL;
    }
    while (str[retval] == ' ')
    {
        retval++;
        continue;
    }

    char name[40];
    int retVal2 = getName(str + retval, name);
    if (retVal2 == -1)
    {
        printf("Неверная инструкция\n");
        return NULL;
    }
    retval += retVal2;
    command = choiseCommand(name);
    while (str[retval] == ' ')
    {
        retval++;
        continue;
    }

    if (command != 43)
    {
        retval = getNum(str + retval, &operand);
        if (retval == -1)
        {
            printf("Неверный операнд\n");
            return NULL;
        }
    }
    else
    {
        operand = 0;
    }

    return newCommand(number, command, operand);
}
int commandEncode(int command, int operand, int *value)
{
    if ((command < 10) || (command > 11 && command < 20) || (command > 21 && command < 30) || (command > 33 && command < 40) || (command > 43 && command < 51) || (command > 76))
    {
        return -1;
    }
    if (operand > 127 || operand < 0)
    {
        return -2;
    }
    *value = (command << 7) | operand;
    return 0;
}