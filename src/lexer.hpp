#pragma once
#include <string>
#include <vector>

namespace gehu {
    enum class TokenType { Let, Identifier, Assign, String, Show, Semicolon, Eof };
    struct Token {
        TokenType type;
        std::string text;
    };
    class Lexer {
        std::string src;
        size_t pos = 0;
    public:
        Lexer(const std::string& s) : src(s) {}
        std::vector<Token> tokenize();
    };
} 