

#include "symbol_tableDef.h"
#include "error_handlerDef.h"
#include "astDef.h"

SymbolTable* initializeSymbolTable(int numberSlots,Scope s);
int symbolHashFunction(SymbolTable* st ,char* str);
void addSymbolEntry(SymbolTable* st, SymbolEntry* entry,ErrorList* els);
SymbolEntry* addEntryToList(SymbolEntry* list, SymbolEntry* s);
SymbolEntry* createSymbolEntry(Token* symbolToken,SymbolLabel symbolLabel);
SymbolEntry* searchSymbolEntry(SymbolEntry* ls, Token* token);
SymbolEntry* lookupSymbolEntry(SymbolTable* st, Token* token);

void constructSymbolTableHelper(ASTNode* node, SymbolTable* st,ErrorList* els);
SymbolTable* constructSymbolTable(AST* ast,ErrorList* els);
