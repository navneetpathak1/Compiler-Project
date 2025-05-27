#pragma once
#include "parser.hpp"
#include <map>
#include <string>
#include <memory>

namespace gehu {
    class CodeGen {
        std::map<std::string, std::string> vars;
    public:
        void generate(const std::vector<std::shared_ptr<Stmt>>& stmts);
        void run();
    };
} 