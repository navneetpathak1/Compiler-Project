// 
//Parser class definition
//Parser class is responsible for parsing the source code
//It reads the tokens and creates an AST
//It also handles errors
//It also handles whitespace, comments, and other special characters
//It also handles strings, numbers, and identifiers
//It also handles operators, delimiters, and special characters
//It also handles keywords
//It also handles errors
#pragma once

#include "lexer.hpp"
#include "ast.hpp"
#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();

private:
    std::vector<Token> tokens;
    size_t current;

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseVariableDeclaration();
    std::unique_ptr<Statement> parseShowStatement();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseAssignmentStatement();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parsePrimary();
    
    bool match(TokenType type);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, const std::string& message);
}; 