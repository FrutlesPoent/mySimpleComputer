#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "parser.h"

int main(int argc, char** argv){
    if (argc != 3){
        fprintf(stderr, "Wrong arguments\n"); // exit thread
        return -1;
    }

    FILE* in, *out;

    in = fopen(argv[1], "r");
    if (in == NULL){
        fprintf(stderr, "Error while opening file\n");
        return -1;
    }

    out = fopen(argv[2], "wb");
    if (out == NULL){
        fprintf(stderr, "Error while creating file");
        return -1;
    }

    char *string = malloc(sizeof(char) * 1000);
    Command **commands = calloc(100, sizeof(Command *)); // array for commands
    int stringNumber = 1;

    while(!feof(in))
    {
        int retval = readLine(in, string);
        if (retval == 0)
            continue;
        Command *command = parse(string);
        if (command == NULL) 
        {
            fprintf(stderr, "Error on %d string\n", stringNumber);
            return -1;
        }

        if (command -> memoryNumber >= 100){ // if more or equal 100
            fprintf(stderr, "Wrong memory on %d string\n", stringNumber);
            return -1;
        }

        commands[command -> memoryNumber] = command; // input command in array of commands
        stringNumber++; // increase number of strings that we read
    }

    int *memory = calloc(SIZE, sizeof(int));
    for (int i = 0; i < SIZE; i++)
    {
        if (commands[i] == NULL)
        {
            memory[i] = 0;
        }
        else
        {
            Command *comm = commands[i];
            if (comm->command == 0)
            {
                if (comm->operand > -1 * (int)~(1 << 15))
                {
                    printf("Wrong variable on %d\n", i);
                    return -1;
                }
                memory[i] = comm->operand + 32768;
            }
            else
            {
                int retVal = sc_commandEncode(comm->command, comm->operand, &memory[i]);
                if (retVal != 0)
                {
                    printf("Wrong command on %d\n", i);
                    return -1;
                }
            }
        }
    }
    fwrite(memory, sizeof(int), SIZE, out);
    return 0;
}