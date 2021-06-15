#ifndef TREE_H
#define TREE_H
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "instruction.h"
#include <stdlib.h>

typedef char (*NodeFunc)(char,char);

typedef struct _Node {
    char data;
    int literalLocation;
    struct _Node* left; // pointer to left part of tree
    struct _Node* right; // pointer to the right part of tree
    NodeFunc operation;
}Node;

// typedef struct _OpReturn {
//     int index;
//     Node* node;
// } OpReturn;

Node* createTree(char* func);
char eval(Node* root);
char evals(char* func);

#endif