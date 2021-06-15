#include "instruction.h"
#include <stdlib.h>

int currentLine; // line that we currently reading

static Instruction* instruction; // global structure for keeping instructions

void initInstruction(){
    instruction = malloc(sizeof(Instruction));
    instruction->variables = NULL;
    instruction->instructions = NULL;
    instruction->instructionStack = 0;
    instruction->variableStack = SIZE - 1;
    instruction->listSize = 0;
    instruction->instructionListSize = 0;
    instruction->listCapacity = 0;
    instruction->instructionListCapacity = 0;
    instruction->varName = 0;
    instruction->lastLitName = -127;
    currentLine = -1;
}

Variable* newVarLit(int location, int value, char var) {
    Variable* varStruct = malloc(sizeof(Variable));
    if (varStruct == NULL)
        return varStruct;
    varStruct->varName = var;
    varStruct->location = location;
    varStruct->isAccum = 0;
    varStruct->isLiterall = 1;
    varStruct->literalVal = value;
    return varStruct;
}

int setString(int string){
    if (string <= currentLine) { // if next string less than previously
        return -1;  // than error
    }
    currentLine = string;
    return 0;
}

Variable* findVarS(char var, char storeAccum) { // search variables in list
    for (int i = 0; i < instruction->listSize; i++) {
        if (storeAccum){
            instruction->variables[i]->isAccum = 0; //if it's in accum
        }
        if (instruction->variables[i]->varName == var) { // if we got a match
            return instruction->variables[i];
        }
    }
    return NULL;
}

Variable* newVar(char var) {
    Variable* varStruct = malloc(sizeof(Variable));
    if (varStruct == NULL)
        return varStruct;
    varStruct->varName = var;
    varStruct->isLiterall = 0; // is number or symbol
    varStruct->location = -2; // unknown location
    varStruct->isAccum = 0;
    return varStruct;
}

Variable* newVarSec(char var, int location) {
    Variable* varStruct = newVar(var); // create var as structure 
    varStruct->location = location; // set location
    return varStruct;
}


int addVariable(char var) {
    if (instruction->variables == NULL) { // if null 
        instruction->variables = malloc(sizeof(Variable*) * 10); // give memory
        instruction->listCapacity = 10;
    }
    if (instruction->listSize == instruction->listCapacity) { // if size == capacity then more memory need
        instruction->variables = realloc(instruction->variables, sizeof(Variable) * instruction->listSize + 10);
        instruction->listCapacity += 10;
    }
    if (findVarS(var, 0) == NULL) { // if we didn't find var and it's not in accum
        if (instruction->variableStack == instruction->instructionStack) // if stack full
            return -2;
        //add new var to list
        instruction->variables[instruction->listSize++] = newVarSec(var, instruction->variableStack--); // create var as string
        return 0;
    }

    return -1;
}

Inst* newInstruction(String* instruction, int location) {
    Inst* inst = malloc(sizeof(Inst)); // create structure
    if (inst == NULL)
        return inst;
    inst->instruction = instruction; // instructions
    inst->location = location; // instruction's location
    inst->isPromise = 0;
    inst->line = currentLine; // instruction place
    return inst;
}

void addInstructionSecond(char* inst, int operand) { // operand
    String* instString = newStringFirst(inst); // create String
    char addresString[4]; // addres number
    char varString[4]; // var addres number
    int addres = instruction->instructionStack++;
    addresString[0] = (addres / 10) + '0'; 
    addresString[1] = (addres % 10) + '0';
    addresString[2] = ' ';
    addresString[3] = '\0';

    varString[0] = ' '; 
    varString[1] = (operand / 10) + '0';
    varString[2] = (operand % 10) + '0';
    varString[3] = '\0';

    instString = stringPrependOne(instString, addresString); // add with checking if we have already something in String
    instString = stringAppendOne(instString, varString);

    if (instruction->instructions == NULL)
    {
        instruction->instructions = malloc(sizeof(Inst) * 10);
        instruction->instructionListCapacity = 10;
    }
    if (instruction->instructionListSize == instruction->instructionListCapacity)
    {
        instruction->instructions = realloc(instruction->instructions,
                                        sizeof(Inst) * (instruction->instructionListSize + 10));
        instruction->instructionListCapacity += 10;
    }
    instruction->instructions[instruction->instructionListSize++] = newInstruction(instString, addres);
}

void addInstructionFirst(char* inst, char var){
    Variable* varStructure = findVarS(var, 0); // trying to find var
    addInstructionSecond(inst, varStructure->location);
}

void addInstructionThird(char* inst, int offset) {
    addInstructionSecond(inst, instruction->instructionStack + offset); 
}

void addInstructionPromise(char* instructionChar, int line) {
    if (instruction->instructions == NULL) {
        instruction->instructions = malloc(sizeof(Inst) * 10);
        instruction->instructionListCapacity = 10;
    }
    if (instruction->instructionListSize == instruction->instructionListCapacity)
    {
        instruction->instructions = realloc(instruction->instructions,
                                        sizeof(Inst) * (instruction->instructionListSize + 10));
        instruction->instructionListCapacity += 10;
    }
    String* instruct = newStringFirst(instructionChar);

    char addresString[4];
    int addres = instruction->instructionStack++;
    addresString[0] = (addres / 10) + '0';
    addresString[1] = (addres % 10) + '0';
    addresString[2] = ' ';
    addresString[3] = '\0';
    stringPrependOne(instruct, addresString);

    Inst* ins = newInstruction(instruct, 0);
    ins->promiseData = line;
    ins->line = -1;
    ins->isPromise = 1;
    instruction->instructions[instruction->instructionListSize++] = ins;

}

void moveAccum(char var) {
    Variable* varStructure = findVarS(var, 1);
    addInstructionSecond("LOAD", varStructure->location); // move to accum 
    varStructure->isAccum = 1;
}

void storeAccum(char var) {
    Variable* varStructure = findVarS(var, 1);
    addInstructionSecond("STORE", varStructure->location); // load from accum
}

int variableInAccum(char var) {
    Variable* varStructure = findVarS(var, 0);
    if (varStructure == NULL)
        return 1;
    return varStructure->isAccum;
}

int instructionLocationByString(int string) {
    for (int i = 0; i < instruction->instructionListSize; i++) {
        if (instruction->instructions[i]->line == string)
            return instruction->instructions[i]->location;
    }
    return -1; // unknown location
}

void addInstructionEnd() {
    int addres = instruction->instructionStack;
    char addresString[4];

    addresString[0] = (addres / 10) + '0';
    addresString[1] = (addres % 10) + '0';
    addresString[2] = ' ';
    addresString[3] = '\0';

    String* instString = newStringFirst(addresString);
    instString = stringAppendOne(instString, "HALT");

    if (instruction->instructions == NULL) {
        instruction->instructions = malloc(sizeof(Inst) * 10);
        instruction->instructionListCapacity = 10;
    }

    if (instruction->instructionListSize == instruction->instructionListCapacity) {
        instruction->instructions = realloc(instruction->instructions, sizeof(Inst) * (instruction->instructionListSize + 10));
        instruction->instructionListCapacity += 10;
    }

    instruction->instructions[instruction->instructionListSize++] = newInstruction(instString, addres);
}

void popVar(){
    instruction->varName--;
    free(instruction->variables[instruction->listSize - 1]);
    instruction->listSize--;
    instruction->variableStack++;
}

int isVar(int var) {
    Variable* varStruct = findVarS(var, 0);
    if (varStruct->varName < 'A' && varStruct->isLiterall == 0) // is it number or symbol
        return 1;
    else
        return 0;
}

int pushVar(){
    addVariable(instruction->varName);
    return instruction->varName++;
}

void writeInstruction(FILE* fd) {
    for (int i = 0; i < instruction->instructionListSize; i++) {
        // String* inst = instruction->instructions[i]->instruction;
        Inst* ins = instruction->instructions[i];
        if (ins->isPromise) {
            int addr = instructionLocationByString(ins->promiseData);
            char varStr[4];
            varStr[0] = ' ';
            varStr[1] = (addr / 10) + '0';
            varStr[2] = (addr % 10) + '0';
            varStr[3] = '\0';
            stringAppendOne(ins->instruction, varStr);
        }
        String* inst = ins->instruction;
        for (int j = 0; j < inst->length; j++){
            fputc(inst->string[j], fd);
        }
        fputc('\n', fd);
    }
}

Variable* findLiteral(int val) {
    for (int i = 0; i < instruction->listSize; i++) {
        if (instruction->variables[i]->isLiterall) {
            if (instruction->variables[i]->literalVal == val)
                return instruction->variables[i];
        }
    }
    return NULL;
}

int getLiteralLocation(int val) {
    Variable* var = findLiteral(val);
    return  var->location;
}

void addLiteral(int val) {
    if (instruction->variables == NULL) {
        instruction->variables = malloc(sizeof (Variable*) * 10);
        instruction->listCapacity = 10;
    }
    if(instruction->listSize == instruction->listCapacity) {
        instruction->variables = realloc(instruction->variables, sizeof(Variable) * (instruction->listSize + 10));
        instruction->listCapacity += 10;               
    }
    if(findLiteral(val) == NULL) {
        if (instruction->variableStack == instruction->instructionStack)
            return;
        String* str = newStringFirst(" = ");
        char add[3];
        add[0] = instruction->variableStack / 10 + '0';
        add[1] = instruction->variableStack % 10 + '0';
        add[2] = '\0';

        char var[5];
        int pVal = val;
        // var[0] = '-';
        var[0] = pVal / 1000 + '0';
        var[1] = pVal / 100 % 10 + '0';
        var[2] = pVal / 10 % 10 + '0';
        var[3] = pVal % 10 + '0';
        var[4] = '\0';
        stringAppendOne(str, var);
        stringPrependOne(str, add);

        if (instruction->instructions == NULL) {
            instruction->instructions = malloc(sizeof(Inst) * 10);
            instruction->instructionListCapacity = 10;
        }

        if (instruction->instructionListSize == instruction->instructionListCapacity){
            instruction->instructionListCapacity += 10;
        }
        instruction->instructions[instruction->instructionListSize++] = newInstruction(str, instruction->variableStack);
        instruction->variables[instruction->listSize++] = newVarLit(instruction->variableStack--, val, instruction->lastLitName++);
        return;
        
    }

    return;
}

char getLitName(int val) {
    Variable* var = findLiteral(val);
    return var->varName;
}

void findLiterals(char* str) {
    for (int i = 0; i < strlen(str); i++)
    {
        if (isupper(str[i])) {
            if (isupper(str[i + 1]))
                return;
            else
                continue;
        } else if (isdigit(str[i])) {
            char number[7];
            int nIdx = 0;
            int j = i;
            for (; j < strlen(str); j++) {
                if (str[j] == ' ' || str[j] == '\0' || str[j] == '\n') {
                    break;
                }
                number[nIdx++] = str[j];
            }
            number[nIdx] = '\0';
            int val = atoi(number);
            addLiteral(val);
            i = j + 1;
        }
    }
}




