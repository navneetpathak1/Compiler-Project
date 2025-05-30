//Lexer class definition

//Lexer class is responsible for tokenizing the source code

//It reads the source code character by character and creates tokens

//It also handles whitespace, comments, and other special characters

//It also handles strings, numbers, and identifiers

//It also handles operators, delimiters, and special characters

//It also handles keywords

//It also handles errors



#pragma once



#include <string>

#include <vector>

#include <memory>



enum class TokenType {

   // Keywords

   LET,

   SHOW,

   IF,

   ELSE,



   // Literals

   IDENTIFIER,

   STRING_LITERAL,

   NUMBER_LITERAL,



   // Operators

   EQUALS,

   SEMICOLON,

   PLUS,

   MINUS,

   MULTIPLY,

   DIVIDE,

   GREATER_THAN,

   LESS_THAN,

   GREATER_EQUAL,

   LESS_EQUAL,

   EQUAL_EQUAL,

   NOT_EQUAL,



   // Delimiters

   LEFT_BRACE,

   RIGHT_BRACE,

   LEFT_PAREN,

   RIGHT_PAREN,



   // Special

   EOF_TOKEN,

   ERROR

};



struct Token {

   TokenType type;

   std::string value;

   size_t line;

   size_t column;



   // Default constructor

   Token() : type(TokenType::ERROR), line(0), column(0) {}



   // Parameterized constructor

   Token(TokenType t, const std::string& v, size_t l, size_t c)

       : type(t), value(v), line(l), column(c) {}

};



//Lexer class

//Lexer class is responsible for tokenizing the source code

//It reads the source code character by character and creates tokens

//It also handles whitespace, comments, and other special characters

//It also handles strings, numbers, and identifiers

//It also handles operators, delimiters, and special characters

//It also handles keywords

//It also handles errors

class Lexer {

public:

   explicit Lexer(const std::string& source);

   Token nextToken();

   bool hasNext() const;



private:

   std::string source;

   size_t position;

   size_t line;

   size_t column;



   char current() const;

   char advance();

   void skipWhitespace();

   Token makeToken(TokenType type, const std::string& value);

   Token scanIdentifier();

   Token scanString();

   Token scanNumber();

}; 

