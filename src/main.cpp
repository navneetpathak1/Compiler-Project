#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"
#include "codegen.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

using namespace gehu;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    try {
        // Read source file
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }

        std::string source((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());

        // Lexical analysis
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        // Parsing
        Parser parser(tokens);
        std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

        // Semantic analysis
        SemanticAnalyzer analyzer;
        analyzer.analyze(statements);

        // Code generation
        CodeGen generator;
        generator.generate(statements);
        generator.run();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 