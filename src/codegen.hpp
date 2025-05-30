#pragma once

#include "ast_visitor.hpp"
#include <llvm/IR/LLVMContext.h> // store the LLVM context
#include <llvm/IR/Module.h> // store the LLVM module
#include <llvm/IR/IRBuilder.h> // build the LLVM IR
#include <llvm/IR/Verifier.h> // verify the LLVM IR
#include <llvm/ExecutionEngine/ExecutionEngine.h> // execute the LLVM IR
#include <llvm/ExecutionEngine/GenericValue.h> // store the LLVM generic value
#include <llvm/Support/TargetSelect.h> // select the target
#include <map> // store the variables
#include <string> // store the variable names

// inherit from ASTVisitor
class CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();
    void generate(Program* program);
    void run();

    // Visitor methods
    void visitStringLiteral(StringLiteral* node) override;
    void visitNumberLiteral(NumberLiteral* node) override;
    void visitIdentifier(Identifier* node) override;
    void visitBinaryExpression(BinaryExpression* node) override;
    void visitBlock(Block* node) override;
    void visitIfStatement(IfStatement* node) override;
    void visitVariableDeclaration(VariableDeclaration* node) override;
    void visitShowStatement(ShowStatement* node) override;
    void visitAssignmentStatement(AssignmentStatement* node) override;

private:
    void createPrintfFunction();
    
    std::unique_ptr<llvm::LLVMContext> context; // store the LLVM context
    std::unique_ptr<llvm::Module> module; // store the LLVM module
    std::unique_ptr<llvm::IRBuilder<>> builder; // build the LLVM IR
    llvm::Function* printfFunction; // store the printf function
    std::map<std::string, llvm::Value*> variables; // store the variables
    llvm::Value* currentValue; // store the current value
}; 