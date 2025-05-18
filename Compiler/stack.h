

#include "stackDef.h"

StackNode* createStackNode(NaryTreeNode* ntn);

// Stack operations
void push(Stack* st,NaryTreeNode* ntn);
NaryTreeNode* top(Stack* st);
void pop(Stack* st);
Stack* initialiseStack(ParseTree* pt);
void pushTreeChildren(Stack* st,NaryTreeNode* ntn);
