

#include "code_genDef.h"


void printLeaf(ASTNode* leafNode,FILE* f);

void codeGenerationHelper(ASTNode* node, SymbolTable* st, FILE* f,int recordArithmetic); // recordArithmetic(1) indicates a record operation
void codeGeneration(AST* ast, SymbolTable* st, FILE* f);
