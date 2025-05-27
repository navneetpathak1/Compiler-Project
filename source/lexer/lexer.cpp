#include "lexer.hpp"
#include <cctype>
#include <stdexcept>

namespace gehu {

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::advance() {
    char c = source[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    column++;
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token Lexer::makeToken(TokenType type) const {
    return Token(type, source.substr(start, current - start), line, column);
}

Token Lexer::errorToken(const std::string& message) const {
    return Token(TokenType::Error, message, line, column);
}

Token Lexer::number() {
    while (isdigit(peek())) advance();
    
    // Look for decimal point
    if (peek() == '.' && isdigit(peekNext())) {
        advance(); // consume the '.'
        while (isdigit(peek())) advance();
    }
    
    return makeToken(TokenType::Number);
}

Token Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }
    
    if (isAtEnd()) {
        return errorToken("Unterminated string.");
    }
    
    // The closing "
    advance();
    
    // Trim the surrounding quotes
    std::string value = source.substr(start + 1, current - start - 2);
    Token token = makeToken(TokenType::String);
    token.literal = value;
    return token;
}

Token Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();
    
    std::string text = source.substr(start, current - start);
    TokenType type;
    
    if (text == "let") type = TokenType::Let;
    else if (text == "show") type = TokenType::Show;
    else if (text == "match") type = TokenType::Match;
    else if (text == "spawn") type = TokenType::Spawn;
    else if (text == "wait") type = TokenType::Wait;
    else if (text == "if") type = TokenType::If;
    else if (text == "else") type = TokenType::Else;
    else if (text == "while") type = TokenType::While;
    else if (text == "for") type = TokenType::For;
    else if (text == "return") type = TokenType::Return;
    else type = TokenType::Identifier;
    
    return makeToken(type);
}

Token Lexer::unit() {
    while (isalpha(peek())) advance();
    
    std::string unit = source.substr(start, current - start);
    Token token = makeToken(TokenType::Unit);
    token.literal = unit;
    return token;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        start = current;
        skipWhitespace();
        
        if (isAtEnd()) break;
        
        char c = advance();
        if (isdigit(c)) {
            tokens.push_back(number());
            continue;
        }
        
        if (isalpha(c)) {
            tokens.push_back(identifier());
            continue;
        }
        
        switch (c) {
            case '(': tokens.push_back(makeToken(TokenType::LeftParen)); break;
            case ')': tokens.push_back(makeToken(TokenType::RightParen)); break;
            case '{': tokens.push_back(makeToken(TokenType::LeftBrace)); break;
            case '}': tokens.push_back(makeToken(TokenType::RightBrace)); break;
            case ';': tokens.push_back(makeToken(TokenType::Semicolon)); break;
            case ',': tokens.push_back(makeToken(TokenType::Comma)); break;
            case '.': tokens.push_back(makeToken(TokenType::Dot)); break;
            case '-': 
                if (match('>')) tokens.push_back(makeToken(TokenType::Arrow));
                else tokens.push_back(makeToken(TokenType::Minus));
                break;
            case '+': tokens.push_back(makeToken(TokenType::Plus)); break;
            case '*': tokens.push_back(makeToken(TokenType::Star)); break;
            case '/': tokens.push_back(makeToken(TokenType::Slash)); break;
            case '|':
                if (match('>')) tokens.push_back(makeToken(TokenType::Pipeline));
                else tokens.push_back(errorToken("Expected '>' after '|'"));
                break;
            case '!':
                if (match('=')) tokens.push_back(makeToken(TokenType::NotEqual));
                else tokens.push_back(makeToken(TokenType::Not));
                break;
            case '=':
                if (match('=')) tokens.push_back(makeToken(TokenType::Equal));
                else tokens.push_back(makeToken(TokenType::Assign));
                break;
            case '<':
                if (match('=')) tokens.push_back(makeToken(TokenType::LessEqual));
                else tokens.push_back(makeToken(TokenType::Less));
                break;
            case '>':
                if (match('=')) tokens.push_back(makeToken(TokenType::GreaterEqual));
                else tokens.push_back(makeToken(TokenType::Greater));
                break;
            case '"': tokens.push_back(string()); break;
            default:
                tokens.push_back(errorToken("Unexpected character."));
        }
    }
    
    tokens.push_back(Token(TokenType::Eof, "", line, column));
    return tokens;
}

} // namespace gehu 