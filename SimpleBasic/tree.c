#include "tree.h"

Node* createNode() {
    Node* node = malloc(sizeof(*node));
    node->left = NULL;
    node->right = NULL;
    node->operation = NULL;
    node->data = -1;
    node->literalLocation = -1;
    return node;
}

struct OpReturn {
    int index;
    Node* node;
};

int isOp(char symbol) {
    switch (symbol) {
        case '*':
        case '-':
        case '+':
        case '/':
            return 1;
        default:
            return 0;

    }
}

int isOpMore(char op1, char op2){
    if (op1 == '+' || op1 == '-'){
        if (op2 == '*' || op2 == '/')
            return 1;
        else
            return 0;
    }
    return -1;
}

char getAccum(char var1, char var2) {
    char accum = -1;
    if (variableInAccum(var1))
        accum = var1;
    else if (variableInAccum(var2))
        accum = var2;
    if (accum == -1) {
        moveAccum(var1);
        accum = var1;
    }

    return accum;
}

char multiply(char leftValue, char rightValue) {
    char accum = getAccum(leftValue, rightValue);
    if (accum == leftValue)
        addInstructionFirst("MUL", rightValue);
    else
        addInstructionFirst("MUL", leftValue);
    if (isVar(leftValue)) {
        popVar();
        popVar();
    }

    return accum;
}

char divide(char leftValue, char rightValue) {
    char accum = getAccum(leftValue, rightValue);
    if (accum == leftValue)
        addInstructionFirst("DIVIDE", rightValue);
    else
        addInstructionFirst("DIVIDE", leftValue);
    if (isVar(leftValue)) {
        popVar();
        popVar();
    }

    return accum;
}

char plus(char leftValue, char rightValue) {
    char accum = getAccum(leftValue, rightValue);
    if (accum == leftValue)
        addInstructionFirst("ADD", rightValue);
    else
        addInstructionFirst("ADD", leftValue);
    if (isVar(leftValue)) {
        popVar();
        popVar();
    }

    return accum;
}

char minus(char leftValue, char rightValue) {
    char accum = getAccum(leftValue, rightValue);
    if (accum == leftValue)
        addInstructionFirst("SUB", rightValue);
    else
        addInstructionFirst("SUB", leftValue);
    if (isVar(leftValue)) {
        popVar();
        popVar();
    }

    return accum;
}

void setOp(Node* node, char symbol) {
    switch(symbol) {
        case '*':
            node->operation = multiply;
            break;
        case '/':
            node->operation = divide;
            break;
        case '+':
            node->operation = plus;
            break;
        case '-':
            node->operation = minus;
            break;
    }
}

struct OpReturn calcOperation(char* func) {
    Node* node = createNode();
    int i;
    for (i = 0; func[i] != ')' && func[i] != '\0'; i++) {
        if (func[i] == ' ') // if we hit space then continue
            continue;
        if (isupper(func[i]) || isdigit(func[i])) {
            if (node->operation == NULL){ // if we don't have in node operation
                node->left = createNode(); // than we create new one to put data in him
                if (isdigit(func[i])) {
                    char buff[8];
                    int index = 0;
                    for (; func[i] != ' ' && func[i] != '\0' && func[i] != ')'; i++) {
                        buff[index++] = func[i];
                    }
                    buff[index] = '\0';
                    int val = atoi(buff);
                    int location = getLiteralLocation(val);
                    node->left->literalLocation = location;
                    node->left->data = getLitName(val);
                    i--;
                } else
                    node->left->data = func[i];
            } else { // else we create to the right
                node->right = createNode();
                if (isdigit(func[i])) {
                    char buff[8];
                    int index = 0;
                    for (; func[i] != ' ' && func[i] != '\0' && func[i] != ')'; i++) {
                        buff[index++] = func[i];
                    }
                    buff[index] = '\0';
                    int val = atoi(buff);
                    int location = getLiteralLocation(val);
                    node->right->literalLocation = location;
                    node->right->data = getLitName(val);
                    i--;

                } else 
                     node->right->data = func[i];
            }
        } else if (isOp(func[i])) { // if it's not instruction 
            if (node->operation == NULL) {
                setOp(node, func[i]);
                node->data = func[i];
            } else {
                if(isOpMore(node->data, func[i])) { // check op in data and in func[i]
                    Node* tmp = node->right;
                    struct OpReturn ret = calcOperation(&func[i]);
                    node->right = ret.node;
                    i += ret.index;
                    Node* left = node->right;
                    while(left->right) // while we have right node
                        left = left->left; // left part of tree
                    left->left = tmp;
                } else { // if prior lower than 
                    Node* tmp = node; 
                    node = createNode();
                    setOp(node, func[i]); // add instruction
                    node->data = func[i]; 
                    node->left = tmp;
                }
            }
        } else if (func[i] == '(') { // if it's start of math calculations
            struct OpReturn ret = calcOperation(&func[i + 1]); // goto next symbol to check
            if (node->operation == NULL){
                node->left = ret.node; // if op is empty than place in left
            }else{ // else in right 
                node->right = ret.node;
            }
            i += ret.index + 1; // increase index to move next
        }
    }

    struct OpReturn ret;
    ret.index = i;
    ret.node = node;
    return ret;
}



Node* createTree(char* func) {
    struct OpReturn ret = calcOperation(func);
    return ret.node; 
}

char eval(Node* root) {
    char leftValue, rightValue;
    char leftValueTmp = -1;
    if (root->left->operation == NULL)
        leftValue = root->left->data; // if op is null in left node
    else
        leftValue = eval(root->left); // if op is not null then move left and check there
    if (root->right->operation == NULL) {
        rightValue = root->right->data;
    } else {
        if (root->left->operation != NULL){
            leftValueTmp = pushVar();
            storeAccum(leftValueTmp);
        }
        rightValue = eval(root->right);
    }
    if (leftValueTmp != -1) {
        int tmp = pushVar();
        storeAccum(tmp);
        moveAccum(leftValueTmp);
        rightValue = tmp;
        leftValue = leftValueTmp;
    }
    return root->operation(leftValue, rightValue);
}

char evals(char* func) {
    Node* root = createTree(func);
    if (root->operation == NULL)
        return root->left->data;
    return eval(createTree(func));
}