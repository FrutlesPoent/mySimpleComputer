#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "inst.h"
#include <malloc.h>
#include <ctype.h>
#include <string.h>

int readLine(FILE *fd, char* buff){
    char symbol;
    int i = 0;
    while(1) {
        symbol = fgetc(fd);
        if (symbol == EOF)
            break;
        if (symbol == '\n')
            break;
        buff[i++] = symbol;
    }
    buff[i] = '\0';

    return i;
}

int main(int argc, char** argv){
    FILE* in;
    FILE* out;
    initInstruction(); //creating empty list of instruction

    if (argc != 3){
        in = fopen("basic.sb", "r");
        out = fopen("basic.sa", "w");
 
    } else {
        in = fopen(argv[1], "r");
        if (in == NULL){
            printf("Error while opening file\n");
            return -2;
        }
        out = fopen(argv[2], "w");
        if (out == NULL){
            printf("Error while creating the file");
            return -3;
        }
    }
    char* buff = malloc(sizeof(char) * 500);

    while(!feof(in)) {
        int strLen =  readLine(in, buff);
        if (strLen == 0)
            continue;
        int i = 3, j = 0;
        char instr[10];
        int flag = 0;
        for (; i < strLen; i++) {
            if (buff[i] == ' '){
                if (flag)
                    break;
                else
                    continue;
            }
            if (isupper(buff[i])){
                flag = 1;
                instr[j++] = buff[i];
            } else {
                printf("Wrong instruction\n");
                return -4;
            }
            instr[j] = '\0';
            if (!strcmp("LET", instr) || !strcmp("IF", instr)) {
                findLiterals(&buff[i + j]);
            }
        }
    }
    fseek(in, 0, SEEK_SET);

    while(!feof(in)) {
        int stringLenght = readLine(in, buff);//read line from file

        if (stringLenght == 0)
            continue;

        char lineNumber[3];
        lineNumber[0] = buff[0];
        lineNumber[1] = buff[1];
        lineNumber[2] = '\0';
        int string = atoi(lineNumber); // first number
        setString(string); // check for errors in file(queue)
        char instruction[10];
        int j = 0;
        int i = 3;
        int flag = 0;
        for (; i < stringLenght; i++){
            if (buff[i] == ' '){ // if find space
                if (flag) // exit if flag 1
                    break;
                else
                    continue;
            }
            if (isupper(buff[i])){ // if it's symbol of instruction 
                flag = 1;
                instruction[j++] = buff[i]; // input symbol of instruction
            } else {
                printf("Wrong instruction on %d\n", (int)string);
                return -4;
            }
        } // after we read full instruction 
        instruction[j] = '\0';
        int retval = chooseInstruction(instruction, buff + i + 1);
        if (retval) {
            printf("Wrong opperand %d\n", string);
            return -1;
        }
    }
    writeInstruction(out);
    return 0;
}