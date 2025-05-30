//Parser class is responsible for parsing the source code
//It reads the tokens and creates an AST
//It also handles errors
//It also handles whitespace, comments, and other special characters
//It also handles strings, numbers, and identifiers
//It also handles operators, delimiters, and special characters
//It also handles keywords
//It also handles errors
#include "parser.hpp"
#include "errors.hpp"
#include <stdexcept>
#include <iostream>
//Parser class constructor
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}


//main
std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();
    
    while (!isAtEnd()) {
        program->statements.push_back(parseStatement());
    }
    
    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::LET)) {
        return parseVariableDeclaration();
    } else if (match(TokenType::SHOW)) {
        return parseShowStatement();
    } else if (match(TokenType::IF)) {
        return parseIfStatement();
    } else if (check(TokenType::IDENTIFIER)) {
        // Assignment statement
        return parseAssignmentStatement();
    }
    throw ParserError("Unexpected token: " + peek().value, peek().line, peek().column);
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    // Parse condition
    if (!match(TokenType::LEFT_PAREN)) {
        throw ParserError("Expected '(' after 'if'", peek().line, peek().column);
    }
    auto condition = parseExpression();
    if (!match(TokenType::RIGHT_PAREN)) {
        throw ParserError("Expected ')' after if condition", peek().line, peek().column);
    }
    // Parse then block
    if (!match(TokenType::LEFT_BRACE)) {
        throw ParserError("Expected '{' before if body", peek().line, peek().column);
    }
    std::vector<std::unique_ptr<Statement>> thenStatements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        thenStatements.push_back(parseStatement());
    }
    if (!match(TokenType::RIGHT_BRACE)) {
        throw ParserError("Expected '}' after if body", peek().line, peek().column);
    }
    auto thenBlock = std::make_unique<Block>(std::move(thenStatements));
    // Parse else block if present
    std::unique_ptr<Block> elseBlock = nullptr;
    if (match(TokenType::ELSE)) {
        if (!match(TokenType::LEFT_BRACE)) {
            throw ParserError("Expected '{' before else body", peek().line, peek().column);
        }
        std::vector<std::unique_ptr<Statement>> elseStatements;
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            elseStatements.push_back(parseStatement());
        }
        if (!match(TokenType::RIGHT_BRACE)) {
            throw ParserError("Expected '}' after else body", peek().line, peek().column);
        }
        elseBlock = std::make_unique<Block>(std::move(elseStatements));
    }
    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBlock), std::move(elseBlock));
}

std::unique_ptr<Statement> Parser::parseVariableDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::EQUALS, "Expected '=' after variable name");
    auto value = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    return std::make_unique<VariableDeclaration>(name.value, std::move(value));
}

std::unique_ptr<Statement> Parser::parseShowStatement() {
    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after show statement");
    return std::make_unique<ShowStatement>(std::move(expr));
}

std::unique_ptr<Statement> Parser::parseAssignmentStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");
    consume(TokenType::EQUALS, "Expected '=' in assignment");
    auto value = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after assignment");
    return std::make_unique<AssignmentStatement>(name.value, std::move(value));
}

std::unique_ptr<Expression> Parser::parseExpression() {
    return parseComparison();
}

std::unique_ptr<Expression> Parser::parseComparison() {
    auto expr = parseTerm();
    
    while (match(TokenType::GREATER_THAN) || match(TokenType::LESS_THAN) ||
           match(TokenType::GREATER_EQUAL) || match(TokenType::LESS_EQUAL) ||
           match(TokenType::EQUAL_EQUAL) || match(TokenType::NOT_EQUAL)) {
        BinaryOperator op;
        switch (previous().type) {
            case TokenType::GREATER_THAN: op = BinaryOperator::GREATER_THAN; break;
            case TokenType::LESS_THAN: op = BinaryOperator::LESS_THAN; break;
            case TokenType::GREATER_EQUAL: op = BinaryOperator::GREATER_EQUAL; break;
            case TokenType::LESS_EQUAL: op = BinaryOperator::LESS_EQUAL; break;
            case TokenType::EQUAL_EQUAL: op = BinaryOperator::EQUAL_EQUAL; break;
            case TokenType::NOT_EQUAL: op = BinaryOperator::NOT_EQUAL; break;
            default: throw ParserError("Invalid comparison operator", previous().line, previous().column);
        }
        
        auto right = parseTerm();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseTerm() {
    auto expr = parseFactor();
    
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        BinaryOperator op = previous().type == TokenType::PLUS ? BinaryOperator::ADD : BinaryOperator::SUBTRACT;
        auto right = parseFactor();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseFactor() {
    auto expr = parsePrimary();
    
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE)) {
        BinaryOperator op = previous().type == TokenType::MULTIPLY ? BinaryOperator::MULTIPLY : BinaryOperator::DIVIDE;
        auto right = parsePrimary();
        expr = std::make_unique<BinaryExpression>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    if (match(TokenType::STRING_LITERAL)) {
        return std::make_unique<StringLiteral>(previous().value);
    }
    
    if (match(TokenType::NUMBER_LITERAL)) {
        return std::make_unique<NumberLiteral>(std::stoi(previous().value));
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<Identifier>(previous().value);
    }

    // Add support for parenthesized expressions
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }
    
    throw ParserError("Unexpected token in expression: " + peek().value, peek().line, peek().column);
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

//consume token
Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        Token token = advance();
        std::cout << "[Parser] Consumed token: " << token.value << " (Type: " << static_cast<int>(token.type) << ")" << std::endl;
        return token;
    }
    throw ParserError(message, peek().line, peek().column);
} 