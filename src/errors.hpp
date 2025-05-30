#pragma once

#include <stdexcept>
#include <string>

class CompilerError : public std::runtime_error {
public:
    CompilerError(const std::string& message, size_t line, size_t column)
        : std::runtime_error(message), line(line), column(column) {}
    
    size_t getLine() const { return line; }
    size_t getColumn() const { return column; }
    
private:
    size_t line;
    size_t column;
};

class LexerError : public CompilerError {
public:
    LexerError(const std::string& message, size_t line, size_t column)
        : CompilerError("Lexer error: " + message, line, column) {}
};

class ParserError : public CompilerError {
public:
    ParserError(const std::string& message, size_t line, size_t column)
        : CompilerError("Parser error: " + message, line, column) {}
};

class SemanticError : public CompilerError {
public:
    SemanticError(const std::string& message, size_t line, size_t column)
        : CompilerError("Semantic error: " + message, line, column) {}
};

class CodeGenError : public CompilerError {
public:
    CodeGenError(const std::string& message, size_t line, size_t column)
        : CompilerError("Code generation error: " + message, line, column) {}
}; 