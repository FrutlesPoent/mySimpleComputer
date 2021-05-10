#ifndef TREE_H
#define TREE_H

#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef char (*MathFunc)(char, char);

typedef struct _MathNode
{
    char data;
    struct _MathNode *left;
    struct _MathNode *right;
    MathFunc operation;
} MathNode;

MathNode *createTree(char *func);
char eval(MathNode *root);
char evals(char *func);

#endif