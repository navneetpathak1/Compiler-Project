#pragma once
#include "lexer.hpp"
#include <memory>
#include <vector>

namespace gehu {
    struct Stmt { virtual ~Stmt() = default; };
    struct LetStmt : Stmt {
        std::string name, value;
        LetStmt(const std::string& n, const std::string& v) : name(n), value(v) {}
    };
    struct ShowStmt : Stmt {
        std::string name;
        ShowStmt(const std::string& n) : name(n) {}
    };
    class Parser {
        const std::vector<Token>& tokens;
        size_t pos = 0;
    public:
        Parser(const std::vector<Token>& t) : tokens(t) {}
        std::vector<std::shared_ptr<Stmt>> parse();
    };
} 