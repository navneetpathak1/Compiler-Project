

#include "lexerDef.h"

void initializeBuffers(int f);
void initializeLexer(int f);
void removeComments(char* testCaseFile,char* cleanFile);
int getInputStream();
char nextChar();
void retract(int amt);
int rangeMatch(char ch,char start, char end);
int singleMatch(char ch, char chToEqual);
Token* populateToken(Token* t,TokenName tokenName,char* lexeme,int lineNumber,int isNumber,Value* value);
Token* getToken();
void printBuffers();
int stringToInteger(char* str);
float stringToFloat(char* str);
