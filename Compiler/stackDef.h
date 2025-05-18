

#ifndef STACK_
#define STACK_
#include "nary_treeDef.h"

typedef struct StackNode {
    NaryTreeNode* TREE_NODE;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* HEAD;
    int NUM_NODES;
} Stack;

#endif
