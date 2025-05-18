

#include "nary_treeDef.h"

ParseTree* initialiseParseTree();
NaryTreeNode* createLeafNode(int enumId);
NaryTreeNode* createNonLeafNode(int enumId);
NaryTreeNode* createNode(int isTerminal, SymbolType type,NaryTreeNode* parent);
void addRuleToParseTree(NaryTreeNode* ntn, Rule* r);
void printTree(ParseTree* pt);
void printNaryTree(NaryTreeNode* nt);

int getParseTreeNodeCount();
int getParseTreeMemory();
