#include "lexer.hpp"

#include "errors.hpp" //for LexerError

#include <cctype> //for isalpha, isdigit

#include <iostream>



//Lexer class constructor

Lexer::Lexer(const std::string& source)

   : source(source), position(0), line(1), column(1) {}



Token Lexer::nextToken() {

   skipWhitespace();



   if (position >= source.length()) {

       return makeToken(TokenType::EOF_TOKEN, "");

   }



   // Skip comments

   if (current() == '/' && position + 1 < source.length() && source[position + 1] == '/') {

       // Skip until end of line

       while (position < source.length() && current() != '\n') {

           advance();

       }

       skipWhitespace();

       if (position >= source.length()) {

           return makeToken(TokenType::EOF_TOKEN, "");

       }

   }



   char c = current();



   if (isalpha(c)) {

       return scanIdentifier();

   }



   if (isdigit(c)) {

       return scanNumber();

   }



   if (c == '"') {

       return scanString();

   }



   if (c == '=') {

       advance();

       if (current() == '=') {

           advance();

           return makeToken(TokenType::EQUAL_EQUAL, "==");

       }

       return makeToken(TokenType::EQUALS, "=");

   }



   if (c == '!') {

       advance();

       if (current() == '=') {

           advance();

           return makeToken(TokenType::NOT_EQUAL, "!=");

       }

       throw LexerError("Expected '=' after '!'", line, column - 1);

   }



   if (c == '>') {

       advance();

       if (current() == '=') {

           advance();

           return makeToken(TokenType::GREATER_EQUAL, ">=");

       }

       return makeToken(TokenType::GREATER_THAN, ">");

   }



   if (c == '<') {

       advance();

       if (current() == '=') {

           advance();

           return makeToken(TokenType::LESS_EQUAL, "<=");

       }

       return makeToken(TokenType::LESS_THAN, "<");

   }



   if (c == ';') {

       advance();

       return makeToken(TokenType::SEMICOLON, ";");

   }



   if (c == '+') {

       advance();

       return makeToken(TokenType::PLUS, "+");

   }



   if (c == '-') {

       advance();

       return makeToken(TokenType::MINUS, "-");

   }



   if (c == '*') {

       advance();

       return makeToken(TokenType::MULTIPLY, "*");

   }



   if (c == '/') {

       advance();

       return makeToken(TokenType::DIVIDE, "/");

   }



   if (c == '{') {

       advance();

       return makeToken(TokenType::LEFT_BRACE, "{");

   }



   if (c == '}') {

       advance();

       return makeToken(TokenType::RIGHT_BRACE, "}");

   }



   if (c == '(') {

       advance();

       return makeToken(TokenType::LEFT_PAREN, "(");

   }



   if (c == ')') {

       advance();

       return makeToken(TokenType::RIGHT_PAREN, ")");

   }



   // Invalid character

   char invalid = advance();

   throw LexerError("Unexpected character: " + std::string(1, invalid), line, column - 1);

}



bool Lexer::hasNext() const {

   return position < source.length();

}



char Lexer::current() const {

   return position < source.length() ? source[position] : '\0';

}



char Lexer::advance() {

   char c = current();

   position++;

   column++;

   return c;

}



void Lexer::skipWhitespace() {

   while (position < source.length()) {

       char c = current();

       if (c == ' ' || c == '\t') {

           advance();

           continue;

       }

       if (c == '\n') {

           advance();

           line++;

           column = 1;

           continue;

       }

       break;

   }

}



Token Lexer::makeToken(TokenType type, const std::string& value) {

   Token token(type, value, line, column);

   std::cout << "[Lexer] Token: " << value << " (Type: " << static_cast<int>(type) << ")" << std::endl;

   return token;

}



Token Lexer::scanIdentifier() {

   size_t start = position;

   while (position < source.length() && (isalnum(current()) || current() == '_')) {

       advance();

   }



   std::string text = source.substr(start, position - start);



   if (text == "let") {

       return makeToken(TokenType::LET, text);

   }

   if (text == "show") {

       return makeToken(TokenType::SHOW, text);

   }

   if (text == "if") {

       return makeToken(TokenType::IF, text);

   }

   if (text == "else") {

       return makeToken(TokenType::ELSE, text);

   }



   return makeToken(TokenType::IDENTIFIER, text);

}



Token Lexer::scanNumber() {

   size_t start = position;

   while (position < source.length() && isdigit(current())) {

       advance();

   }



   std::string text = source.substr(start, position - start);

   return makeToken(TokenType::NUMBER_LITERAL, text);

}



Token Lexer::scanString() {

   advance(); // Skip opening quote

   size_t start = position;

   size_t startLine = line;

   size_t startColumn = column;



   while (position < source.length() && current() != '"') {

       if (current() == '\n') {

           line++;

           column = 1;

       }

       advance();

   }



   if (position >= source.length()) {

       throw LexerError("Unterminated string literal", startLine, startColumn);

   }



   std::string text = source.substr(start, position - start);

   advance(); // Skip closing quote



   return makeToken(TokenType::STRING_LITERAL, text);

} 

