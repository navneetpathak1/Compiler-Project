#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"
#include "codegen.hpp"
#include "errors.hpp"
#include <fstream>
#include <sstream> //String stream operations
#include <iostream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

//Main function
//argc: Argument count
//argv: Argument vector
//argv[0]: Program name
//argv[1]: Source file name
//argv[2]: Output file name
//argv[3]: Error file name
//argv[4]: Log file name

int main(int argc, char** argv) {
    std::cout << "[main] Program started" << std::endl;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }
    
    try {
        std::cout << "[main] Reading source file..." << std::endl;
        std::string source = readFile(argv[1]);
        std::cout << "[main] Source file read successfully." << std::endl;
        

        std::cout << "[main] Starting lexical analysis..." << std::endl;
        Lexer lexer(source);
        std::vector<Token> tokens;
        Token token;
        do {
            token = lexer.nextToken();
            tokens.push_back(token);
        } while (token.type != TokenType::EOF_TOKEN);
        std::cout << "[main] Lexical analysis complete. Token count: " << tokens.size() << std::endl;
        


        std::cout << "[main] Starting parsing..." << std::endl;
        Parser parser(tokens);
        auto program = parser.parse();
        std::cout << "[main] Parsing complete." << std::endl;
        


        std::cout << "[main] Starting semantic analysis..." << std::endl;
        SemanticAnalyzer analyzer;
        analyzer.analyze(program.get());
        std::cout << "[main] Semantic analysis complete." << std::endl;
        


        std::cout << "[main] Starting code generation..." << std::endl;
        CodeGenerator codegen;
        codegen.generate(program.get());
        std::cout << "[main] Code generation complete. Running program..." << std::endl;
        codegen.run();
        std::cout << "[main] Program execution finished." << std::endl;
        

        
    } catch (const CompilerError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 