#include <stdio.h>
#include "instruction.h"
#include <stdlib.h>
#include <ctype.h>
#include "parse.h"

int readLine(FILE *fd, char *buff)
{
    char sym;
    int i = 0;
    while (1)
    {
        sym = fgetc(fd);
        if (sym == EOF)
            break;
        if (sym == '\n')
            break;
        buff[i++] = sym;
    }
    buff[i] = '\0';
    return i;
}

int main(int argc, char **argv)
{
    FILE *in, *out;
    initContext();
    if (argc != 3)
    {
        in = fopen("file.sb", "r");
        out = fopen("file.sa", "w");
    }
    else
    {
        in = fopen(argv[1], "r");
        if (in == NULL)
        {
            printf("Неудалось открый файл\n");
            return -1;
        }
        out = fopen(argv[2], "w");
        if (out == NULL)
        {
            printf("Неудалось создать файл\n");
            return -1;
        }
    }
    char *buff = malloc(sizeof(char) * 500);
    while (!feof(in))
    {
        int strLen = readLine(in, buff);
        if (strLen == 0)
            continue;
        char lineNumber[3] = {buff[0], buff[1], '\0'};
        int line = atoi(lineNumber);
        setCurrentLine(line);
        char instruction[10];
        int i, j = 0;
        int flag = 0;
        for (i = 3; i < strLen; i++)
        {
            if (buff[i] == ' ')
            {
                if (flag)
                    break;
                else
                    continue;
            }
            if (isupper(buff[i]))
            {
                flag = 1;
                instruction[j++] = buff[i];
            }
            else
            {
                printf("Некорректная инструкция на строке %d\n", line);
                return -1;
            }
        }
        instruction[j] = '\0';
        int retval = choiceInstruction(instruction, (buff + i + 1));
        if (retval != 0)
        {
            printf("Некорректная инструкция на строке %d\n", line);
            return -1;
        }
    }
    writeInstruction(out);
    return 0;
}