#include "parser.hpp"

using namespace gehu;

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> stmts;
    while (pos < tokens.size() && tokens[pos].type != TokenType::Eof) {
        if (tokens[pos].type == TokenType::Let) {
            ++pos;
            std::string name = tokens[pos++].text;
            ++pos; // skip '='
            std::string value = tokens[pos++].text;
            stmts.push_back(std::make_shared<LetStmt>(name, value));
            ++pos; // skip ';'
        } else if (tokens[pos].type == TokenType::Show) {
            ++pos;
            std::string name = tokens[pos++].text;
            ++pos; // skip ';'
            stmts.push_back(std::make_shared<ShowStmt>(name));
        } else {
            ++pos;
        }
    }
    return stmts;
} 