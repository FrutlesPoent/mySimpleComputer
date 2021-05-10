#include "parse.h"

#define SIZE 100

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Неверные аргументы\n");
        return -1;
    }
    FILE *in;
    FILE *out;

    in = fopen(argv[1], "r");
    if (in == NULL)
    {
        printf("Ошибка при открытии файла\n");
        return -1;
    }
    out = fopen(argv[2], "wb");
    if (out == NULL)
    {
        printf("Ошибка при создании файла\n");
        return -1;
    }

    char *line = malloc(sizeof(char) * 1000);
    Command **array = calloc(SIZE, sizeof(Command *));

    int strNum = 1;
    while (!feof(in))
    {
        int retVal = readLine(in, line);
        if (retVal == 0)
            continue;
        Command *comm = parse(line);
        if (comm == NULL)
        {
            printf("Ошибка на строке: %d\n", strNum);
            return -1;
        }
        if (comm->number >= SIZE)
        {
            printf("Неправильная ячейка памяти на строке: %d\n", strNum);
            return -1;
        }
        array[comm->number] = comm;
        strNum++;
    }

    int *memory = calloc(SIZE, sizeof(int));
    for (int i = 0; i < SIZE; i++)
    {
        if (array[i] == NULL)
        {
            memory[i] = 0;
        }
        else
        {
            Command *comm = array[i];
            if (comm->command == 0)
            {
                if (comm->operand > (int)~(1 << 15))
                {
                    printf("Неправильная переменная по адресу: %d\n", i);
                    return -1;
                }
                memory[i] = comm->operand;
            }
            else
            {
                int retVal = commandEncode(comm->command, comm->operand, &memory[i]);
                if (retVal != 0)
                {
                    printf("Неправильная команда по адресу: %d\n", i);
                    return -1;
                }
            }
        }
    }
    fwrite(memory, sizeof(int), SIZE, out);
    return 0;
}
