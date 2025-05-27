#pragma once
#include <string>
#include <vector>
#include <optional>

namespace gehu {

enum class TokenType {
    // Keywords
    Let, Show, Match, Spawn, Wait, If, Else, While, For, Return,
    
    // Operators
    Plus, Minus, Star, Slash, Equal, NotEqual, Less, Greater,
    LessEqual, GreaterEqual, Assign, Pipeline, Not,
    
    // Delimiters
    LeftParen, RightParen, LeftBrace, RightBrace,
    Comma, Semicolon, Dot, Arrow,
    
    // Literals
    Number, String, Unit,
    
    // Identifiers
    Identifier,
    
    // Special
    Eof, Error
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;  // For numbers, strings, units
    size_t line;
    size_t column;
    
    Token(TokenType t, const std::string& l, size_t ln, size_t col)
        : type(t), lexeme(l), line(ln), column(col) {}
};

class Lexer {
    std::string source;
    size_t start = 0;
    size_t current = 0;
    size_t line = 1;
    size_t column = 1;
    
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void skipWhitespace();
    Token makeToken(TokenType type) const;
    Token errorToken(const std::string& message) const;
    Token number();
    Token string();
    Token identifier();
    Token unit();
    
public:
    Lexer(const std::string& source) : source(source) {}
    std::vector<Token> tokenize();
};

} // namespace gehu 