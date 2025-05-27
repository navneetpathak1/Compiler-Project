#include "lexer/lexer.hpp"
#include "parser/ast.hpp"
#include "semantic/type.hpp"
#include "semantic/semantic_visitor.hpp"
#include "codegen/codegen_visitor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    try {
        // Read source file
        std::string source = readFile(argv[1]);
        
        // Lexical analysis
        gehu::Lexer lexer(source);
        auto tokens = lexer.tokenize();
        
        // TODO: Add parser
        // TODO: Add semantic analysis
        // TODO: Add code generation
        
        // For now, just print the tokens
        for (const auto& token : tokens) {
            std::cout << "Token: " << static_cast<int>(token.type) 
                      << " Lexeme: " << token.lexeme 
                      << " Line: " << token.line 
                      << " Column: " << token.column << std::endl;
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 