

#include "semantic_analyzerDef.h"

void semanticAnalysis(AST* ast);

void captureErrors(AST* ast, ErrorList* els);
void captureErrorsHelper(ASTNode* node, ErrorList* els);

TokenListItem* initializeTokenListItem(Token* tk);
TokenListItem* getConditionals(ASTNode* astBooleanExpressionNode);
TokenListItem* mergeConditionals(TokenListItem* leftConditionals, TokenListItem* rightConditionals);
int searchConditionals(TokenListItem* tls, Token* tk);
int searchIterativeChildren(ASTNode* astIterativeStmtNode, TokenListItem* conditionals);
TokenListItem* mergeConditionals(TokenListItem* leftConditionals, TokenListItem* rightConditionals);
