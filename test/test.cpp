#if !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
#error "Exceptions are not enabled! Please ensure -fexceptions is passed to the compiler."
#endif

#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"
#include "code_generator.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <unistd.h>

void testLexer() {
    std::cout << "Testing Lexer..." << std::endl;
    
    std::string source = R"(
        let x = 42;
        let y = "Hello";
        let distance = 5.5 km;
    )";
    
    gehu::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    assert(!tokens.empty());
    std::cout << "Lexer test passed!" << std::endl;
}

void testParser() {
    std::cout << "Testing Parser..." << std::endl;
    
    std::string source = R"(
        let x = 42;
        func add(a: number, b: number): number {
            return a + b;
        }
    )";
    
    gehu::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    gehu::Parser parser(tokens);
    auto statements = parser.parse();
    
    assert(!statements.empty());
    std::cout << "Parser test passed!" << std::endl;
}

void testSemanticAnalyzer() {
    std::cout << "Testing Semantic Analyzer..." << std::endl;
    
    std::string source = R"(
        let x = 42;
        func add(a: number, b: number): number {
            return a + b;
        }
        let result = add(x, 10);
    )";
    
    gehu::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    gehu::Parser parser(tokens);
    auto statements = parser.parse();
    
    gehu::SemanticAnalyzer analyzer;
    analyzer.analyze(statements);
    
    std::cout << "Semantic Analyzer test passed!" << std::endl;
}

void testCodeGenerator() {
    // Read the test.gehu file
    std::ifstream file("test.gehu");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open test.gehu" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Create lexer and scan tokens
    gehu::Lexer lexer(source);
    std::vector<gehu::Token> tokens = lexer.tokenize();

    // Create parser and get AST
    gehu::Parser parser(tokens);
    std::vector<std::shared_ptr<gehu::Stmt>> statements = parser.parse();

    // Create semantic analyzer and analyze
    gehu::SemanticAnalyzer analyzer;
    analyzer.analyze(statements);

    // Create code generator and generate code
    gehu::CodeGenerator generator;
    generator.generate(statements);

    // Run the generated code
    generator.run();
}

void testFullPipeline() {
    std::cout << "Testing Full Pipeline..." << std::endl;
    
    // Print current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }
    // Read test file
    std::ifstream file("../../test/test.gehu");
    assert(file.is_open());
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // Lexical Analysis
    gehu::Lexer lexer(source);
    auto tokens = lexer.tokenize();
    assert(!tokens.empty());
    
    // Parsing
    gehu::Parser parser(tokens);
    auto statements = parser.parse();
    assert(!statements.empty());
    
    // Semantic Analysis
    gehu::SemanticAnalyzer analyzer;
    analyzer.analyze(statements);
    
    // Code Generation
    gehu::CodeGenerator codegen;
    codegen.generate(statements);
    auto module = codegen.getModule();
    assert(module != nullptr);
    
    std::cout << "Full Pipeline test passed!" << std::endl;
}

int main() {
    try {
        testLexer();
        testParser();
        testSemanticAnalyzer();
        testCodeGenerator();
        testFullPipeline();
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
} 