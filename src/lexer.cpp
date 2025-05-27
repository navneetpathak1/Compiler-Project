#include "lexer.hpp"
#include <cctype>

using namespace gehu;

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < src.size()) {
        if (isspace(src[pos])) { ++pos; continue; }
        if (src.compare(pos, 3, "let") == 0 && !isalnum(src[pos+3])) {
            tokens.push_back({TokenType::Let, "let"}); pos += 3; continue;
        }
        if (src.compare(pos, 4, "show") == 0 && !isalnum(src[pos+4])) {
            tokens.push_back({TokenType::Show, "show"}); pos += 4; continue;
        }
        if (src[pos] == '=') { tokens.push_back({TokenType::Assign, "="}); ++pos; continue; }
        if (src[pos] == ';') { tokens.push_back({TokenType::Semicolon, ";"}); ++pos; continue; }
        if (src[pos] == '"') {
            size_t end = src.find('"', pos+1);
            tokens.push_back({TokenType::String, src.substr(pos+1, end-pos-1)});
            pos = end+1; continue;
        }
        if (isalpha(src[pos])) {
            size_t start = pos;
            while (isalnum(src[pos]) || src[pos]=='_') ++pos;
            tokens.push_back({TokenType::Identifier, src.substr(start, pos-start)});
            continue;
        }
        ++pos;
    }
    tokens.push_back({TokenType::Eof, ""});
    return tokens;
} 