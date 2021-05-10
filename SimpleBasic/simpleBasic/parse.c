#include "parse.h"

#define SIZE 100

int currentLine;

int setCurrentLine(int line)
{
    if (line <= currentLine)
    {
        return -1;
    }
    currentLine = line;
    return 0;
}
typedef struct
{
    char name;
    int location;
    int isAccumulator;
    int isLiterall;
} Variable;

typedef struct
{
    String *instruction;
    int line;
    int location;
} Instruction;

typedef struct
{
    Variable **variables;
    Instruction **instructions;
    int varListSize;
    int varListCapacity;
    int instrListSize;
    int instrListCapacity;
    int instrucionStack;
    int variableStack;
    char tempVarName;
} Context;

Context *context;

void initContext()
{
    context = malloc(sizeof(Context));
    context->variables = NULL;
    context->instructions = NULL;
    context->instrucionStack = 0;
    context->variableStack = SIZE - 1;
    context->varListSize = 0;
    context->instrListSize = 0;
    context->varListCapacity = 0;
    context->instrListCapacity = 0;
    context->tempVarName = 0;

    currentLine = -1;
}
Instruction *initInstruction(String *instructionAdd, int location)
{
    Instruction *instruction = malloc(sizeof(Instruction));
    if (instruction == NULL)
        return instruction;
    instruction->instruction = instructionAdd;
    instruction->location = location;
    instruction->line = currentLine;
    return instruction;
}
Variable *initVariable(char name, int location)
{
    Variable *variable = malloc(sizeof(Variable));
    if (variable == NULL)
        return variable;
    variable->name = name;
    variable->isLiterall = 0;
    variable->location = location;
    variable->isAccumulator = 0;
    return variable;
}

static Variable *findVariable(char name, char storeAccumulator)
{
    for (int i = 0; i < context->varListSize; i++)
    {
        if (storeAccumulator)
        {
            context->variables[i]->isAccumulator = 0;
        }
        if (context->variables[i]->name == name)
        {
            return context->variables[i];
        }
    }
    return NULL;
}

int addVariable(char name)
{
    if (context->variables == NULL)
    {
        context->variables = malloc(sizeof(Variable *) * 10);
        context->varListCapacity = 10;
    }
    if (context->varListSize == context->varListCapacity)
    {
        context->variables = realloc(context->variables,
                                     sizeof(Variable) * (context->varListSize + 10));
        context->varListCapacity += 10;
    }
    if (findVariable(name, 0) == NULL)
    {
        if (context->variableStack == context->instrucionStack)
            return -2;
        context->variables[context->varListSize++] = initVariable(name, context->variableStack--);
        return 0;
    }
    return -1;
}
void addInstructionVariable(char *instruction, char name)
{
    Variable *variable = findVariable(name, 0);
    addInstructionOperand(instruction, variable->location);
}

void addInstructionOperand(char *instruction, int operand)
{
    String *instructionString = initString(instruction);
    char addrStr[4];
    char varStr[4];
    int addr = context->instrucionStack++;
    addrStr[0] = (addr / 10) + '0';
    addrStr[1] = (addr % 10) + '0';
    addrStr[2] = ' ';
    addrStr[3] = '\0';

    varStr[0] = ' ';
    varStr[1] = (operand / 10) + '0';
    varStr[2] = (operand % 10) + '0';
    varStr[3] = '\0';

    instructionString = stringPrepend(instructionString, addrStr);
    instructionString = stringAppend(instructionString, varStr);
    if (context->instructions == NULL)
    {
        context->instructions = malloc(sizeof(Instruction) * 10);
        context->instrListCapacity = 10;
    }
    if (context->instrListSize == context->instrListCapacity)
    {
        context->instructions = realloc(context->instructions,
                                        sizeof(Instruction) * (context->instrListSize + 10));
        context->instrListCapacity += 10;
    }
    context->instructions[context->instrListSize++] = initInstruction(instructionString, addr);
}

void addInstructionj(char *instruction, int offset)
{
    addInstructionOperand(instruction, context->instrucionStack + offset);
}

void addInstructionHalt()
{
    int addr = context->instrucionStack;
    char addrStr[4];
    addrStr[0] = (addr / 10) + '0';
    addrStr[1] = (addr % 10) + '0';
    addrStr[2] = ' ';
    addrStr[3] = '\0';
    String *instructionString = initString(addrStr);
    instructionString = stringAppend(instructionString, "HALT");

    if (context->instructions == NULL)
    {
        context->instructions = malloc(sizeof(Instruction) * 10);
        context->instrListCapacity = 10;
    }
    if (context->instrListSize == context->instrListCapacity)
    {
        context->instructions = realloc(context->instructions,
                                        sizeof(Instruction) * (context->instrListSize + 10));
        context->instrListCapacity += 10;
    }
    context->instructions[context->instrListSize++] = initInstruction(instructionString, addr);
}

int variableInAccumulator(char name)
{
    Variable *variable = findVariable(name, 0);
    if (variable == NULL)
        return 1;
    return variable->isAccumulator;
}

void loadAccumulator(char name)
{
    Variable *variable = findVariable(name, 1);
    addInstructionOperand("LOAD", variable->location);
    variable->isAccumulator = 1;
}

void storeAccumulator(char name)
{
    Variable *variable = findVariable(name, 1);
    addInstructionOperand("STORE", variable->location);
}

int pushVariable()
{
    addVariable(context->tempVarName);
    return context->tempVarName++;
}

void popVariable()
{
    context->tempVarName--;
    free(context->variables[context->varListSize - 1]);
    context->varListSize--;
    context->variableStack++;
}

int isVariable(int name)
{
    Variable *variable = findVariable(name, 0);
    if (variable->name < 'A')
        return 1;
    else
        return 0;
}

void writeInstruction(FILE *fd)
{
    for (int i = 0; i < context->instrListSize; i++)
    {
        String *instruction = context->instructions[i]->instruction;
        for (int j = 0; j < instruction->length; j++)
        {
            fputc(instruction->string[j], fd);
        }
        fputc('\n', fd);
    }
}
int instructionLocationByLine(int line)
{
    for (int i = 0; i < context->instrListSize; i++)
    {
        if (context->instructions[i]->line == line)
            return context->instructions[i]->location;
    }
    return -1;
}