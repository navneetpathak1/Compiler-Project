#pragma once
#include "parser.hpp"
#include <set>
#include <stdexcept>

namespace gehu {
    class SemanticAnalyzer {
        std::set<std::string> vars;
    public:
        void analyze(const std::vector<std::shared_ptr<Stmt>>& stmts) {
            for (auto& s : stmts) {
                if (auto let = dynamic_cast<LetStmt*>(s.get()))
                    vars.insert(let->name);
                else if (auto show = dynamic_cast<ShowStmt*>(s.get()))
                    if (!vars.count(show->name)) throw std::runtime_error("Undefined variable: " + show->name);
            }
        }
    };
} 