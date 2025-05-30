#pragma once

#include "ast_visitor.hpp"
#include <map>//for symbol table
#include <string>//for variable names

class SemanticAnalyzer : public ASTVisitor {
public:
    //entry point
    void analyze(Program* program);
    
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
    std::map<std::string, bool> variables;
}; 