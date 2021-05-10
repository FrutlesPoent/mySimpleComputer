#include "parser.h"

int readLine(FILE* fd, char* buff){
    char symbol;
    int i = 0;
    while(1) {
        symbol = fgetc(fd); //return symbol

        if (symbol == EOF)
            break;
        if (symbol == '\n')
            break;
        if (symbol == ';'){
            while ((symbol = fgetc(fd)) != '\n')
                continue;
            break;
        }
        buff[i++] = symbol;
    }
    buff[i] = '\0';
    return i;
}

int getNumber(char* str, int* num){
    int i = 0;
    char numBuff[8];
    while(str[i] != ' ' && str[i] != '\0'){ // while not end
        if (!isdigit(str[i])){
            return -1;
        }
        numBuff[i] = str[i];
        i++;
    }
    if (i == 0)
        return -1;
    numBuff[i] = '\0';
    *num = atoi(numBuff); // string to int
    return i;
}

int getName(char* str, char* name){
    int i = 0;
    while (str[i] != ' '){
        if(isdigit(str[i] || str[i] == '\n'))
            return -1;
        name[i] = str[i];
        i++;
    }

    if (i == 0)
        return -1;
    name[i] = '\0';
    return i;
}

int chooseCommand(char* command){
    if (!strcmp(command,"READ")){
        return 10;
    } else if (!strcmp(command, "WRITE")){
        return 11;
    } else if(!strcmp(command, "LOAD")){
        return 20;
    } else if(!strcmp(command, "STORE")){
        return 21;
    } else if (!strcmp(command, "ADD")){
        return 30;
    } else if (!strcmp(command, "SUB")){
        return 31;
    } else if (!strcmp(command, "DIVIDE")){
        return 32;
    } else if (!strcmp(command, "MUL")){
        return 33;
    } else if (!strcmp(command, "JUMP")){
        return 40;
    } else if (!strcmp(command, "JNEG")){
        return 41;
    } else if (!strcmp(command, "JZ")){
        return 42;
    } else if (!strcmp(command, "HALT")){
        return 43;
    } else if (!strcmp(command, "JC")){
        return 56;
    } else if (!strcmp(command, "=")){
        return 0;
    }
    return -1;
}


Command* parse(char* str){
    int memoryNumber, command, operand;
    int retval = getNumber(str, &memoryNumber);
    if (retval == -1){
        fprintf(stderr, "Wrong memory");
        return NULL;
    }
    while(str[retval] == ' '){
        retval++;
        continue;
    }

    char commandName[40];
    int retval2 = getName(str + retval, commandName);
    if (retval == -1){
        fprintf(stderr, "Wrong command");
        return NULL;
    }

    retval += retval2;
    command = chooseCommand(commandName);
    while(str[retval] == ' '){
        retval += 1;
        continue;
    }

    if (command != 43){ // if not HALT
        retval = getNumber(str + retval, &operand);
        if (retval == -1){
            fprintf(stderr, "Wrong operand");
            return NULL;
        }
    } else {
        operand = 0;
    }

    return createCommand(memoryNumber, command, operand);
}

Command* createCommand(int memoryNumber, int command, int operand){
    Command* commands = malloc(sizeof(*commands));
    if (commands == NULL)
        return commands;
    commands -> memoryNumber = memoryNumber;
    commands -> command = command;
    commands -> operand = operand;

    return commands;
}

int sc_commandEncode(int command, int operand, int* value){

    if ((command < 10) || (command > 11 && command < 20) 
        || (command > 21 && command < 30)
        || (command > 33 && command < 40) 
        || (command > 43 && command < 51)
        || (command > 76)){
            // sc_regSet(COMMAND, 1);
            return -1;
        }
    if (operand > 127 || operand < 0){
        return 1;
    }

    *value = (command << 7) | operand;

    return 0;

}



