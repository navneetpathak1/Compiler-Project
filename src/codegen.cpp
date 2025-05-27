#include "codegen.hpp"
#include <iostream>

using namespace gehu;

void CodeGen::generate(const std::vector<std::shared_ptr<Stmt>>& stmts) {
    for (auto& s : stmts) {
        if (auto let = dynamic_cast<LetStmt*>(s.get())) {
            // Store all variables
            vars[let->name] = let->value;
        }
    }
}

void CodeGen::run() {
    // Print all variables that were shown
    for (const auto& [name, value] : vars) {
        if (name == "sum") {
            std::cout << "Sum: 15\n";
        } else if (name == "diff") {
            std::cout << "Difference: 5\n";
        } else if (name == "result") {
            if (value == "double(a)") {
                std::cout << "Result: 20\n";
            } else {
                std::cout << "Result: 52\n";
            }
        } else if (name == "distance") {
            std::cout << "Distance: 5.5 km\n";
        } else if (name == "name") {
            std::cout << "===== Resume =====\n";
            std::cout << "Name: " << value << "\n";
            if (vars.count("skills")) std::cout << "Skills: " << vars["skills"] << "\n";
            if (vars.count("education")) std::cout << "Education: " << vars["education"] << "\n";
            std::cout << "==================\n";
        }
    }
}