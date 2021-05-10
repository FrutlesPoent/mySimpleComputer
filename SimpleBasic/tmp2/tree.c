#include "tree.h"

MathNode *initNode()
{
    MathNode *node = malloc(sizeof(MathNode));
    node->left = NULL;
    node->right = NULL;
    node->operation = NULL;
    node->data = -1;
    return node;
}

int isOperation(char sym)
{
    switch (sym)
    {
    case '*':
    case '+':
    case '-':
    case '/':
        return 1;
    default:
        return 0;
    }
}

int isOperationBigger(char operation1, char operation2)
{
    if (operation1 == '+' || operation1 == '-')
    {
        if (operation2 == '*' || operation2 == '/')
            return 1;
        else
            return 0;
    }
    else
    {
        return 0;
    }
}

char getAccumulator(char variable1, char variable2)
{
    char accumulator = -1;
    if (variableInAccum(variable1))
        accumulator = variable1;
    else if (variableInAccum(variable2))
        accumulator = variable2;
    if (accumulator == -1)
    {
        moveAccum(variable1);
        accumulator = variable1;
    }
    return accumulator;
}

char mulFunc(char lValue, char rValue)
{
    char accumulator = getAccumulator(lValue, rValue);
    if (accumulator == lValue)
        addInstructionFirst("MUL", rValue);
    else
        addInstructionFirst("MUL", lValue);
    if (isVar(lValue))
    {
        popVar();
        popVar();
    }
    return accumulator;
}

char sumFunc(char lValue, char rValue)
{
    char accumulator = getAccumulator(lValue, rValue);
    if (accumulator == lValue)
        addInstructionFirst("ADD", rValue);
    else
        addInstructionFirst("ADD", lValue);
    if (isVar(lValue))
    {
        popVar();
        popVar();
    }
    return accumulator;
}

char subFunc(char lValue, char rValue)
{
    char accumulator = getAccumulator(lValue, rValue);
    if (accumulator == lValue)
        addInstructionFirst("SUB", rValue);
    else
        addInstructionFirst("SUB", lValue);
    if (isVar(lValue))
    {
        popVar();
        popVar();
    }
    return accumulator;
}

char divideFunc(char lValue, char rValue)
{
    char accumulator = getAccumulator(lValue, rValue);
    if (accumulator == lValue)
        addInstructionFirst("DIVIDE", rValue);
    else
        addInstructionFirst("DIVIDE", lValue);
    if (isVar(lValue))
    {
        popVar();
        popVar();
    }
    return accumulator;
}

void setOperation(MathNode *node, char operation)
{
    switch (operation)
    {
    case '*':
        node->operation = mulFunc;
        break;
    case '+':
        node->operation = sumFunc;
        break;
    case '-':
        node->operation = subFunc;
        break;
    case '/':
        node->operation = divideFunc;
        break;
    }
}

struct MathReturn
{
    int index;
    MathNode *node;
};

struct MathReturn calcS(char *func)
{
    MathNode *node = initNode();
    int i;
    for (i = 0; func[i] != ')' && func[i] != '\0'; i++)
    {
        if (func[i] == ' ')
            continue;
        if (isupper(func[i]))
        {
            if (node->operation == NULL)
            {
                node->left = initNode();
                node->left->data = func[i];
            }
            else
            {
                node->right = initNode();
                node->right->data = func[i];
            }
        }
        else if (isOperation(func[i]))
        {
            if (node->operation == NULL)
            {
                setOperation(node, func[i]);
                node->data = func[i];
            }
            else
            {
                if (isOperationBigger(node->data, func[i]))
                {
                    MathNode *tmp = node->right;
                    struct MathReturn ret = calcS(&func[i]);
                    node->right = ret.node;
                    i += ret.index;
                    MathNode *left = node->right;
                    while (left->left)
                        left = left->left;
                    left->left = tmp;
                }
                else
                {
                    MathNode *tmp = node;
                    node = initNode();
                    setOperation(node, func[i]);
                    node->data = func[i];
                    node->left = tmp;
                }
            }
        }
        else if (func[i] == '(')
        {
            //MathNode* tmp = node->right;
            struct MathReturn ret = calcS(&func[i + 1]);
            if (node->operation == NULL)
                node->left = ret.node;
            else
                node->right = ret.node;
            i += ret.index + 1;
            //node->right->left = tmp;
        }
    }
    struct MathReturn ret;
    ret.index = i;
    ret.node = node;
    return ret;
}

MathNode *createTree(char *func)
{
    struct MathReturn ret = calcS(func);
    return ret.node;
}

char eval(MathNode *root)
{
    char lVal, rVal;
    char lValTmp = -1;
    if (root->left->operation == NULL)
    {
        lVal = root->left->data;
    }
    else
    {
        lVal = eval(root->left);
    }
    if (root->right->operation == NULL)
    {
        rVal = root->right->data;
    }
    else
    {
        if (root->left->operation != NULL)
        {
            lValTmp = pushVar();
            storeAccum(lValTmp);
        }
        rVal = eval(root->right);
    }
    if (lValTmp != -1)
    {
        int tmp = pushVar();
        storeAccum(tmp);
        moveAccum(lValTmp);
        rVal = tmp;
        lVal = lValTmp;
    }
    return root->operation(lVal, rVal);
}

char evals(char *func)
{
    MathNode *root = createTree(func);
    if (root->operation == NULL)
        return root->left->data;
    return eval(createTree(func));
}