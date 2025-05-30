#pragma once

#include "ast_forward.hpp"

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visitStringLiteral(StringLiteral* node) = 0;
    virtual void visitNumberLiteral(NumberLiteral* node) = 0;
    virtual void visitIdentifier(Identifier* node) = 0;
    virtual void visitBinaryExpression(BinaryExpression* node) = 0;
    virtual void visitBlock(Block* node) = 0;
    virtual void visitIfStatement(IfStatement* node) = 0;
    virtual void visitVariableDeclaration(VariableDeclaration* node) = 0;
    virtual void visitShowStatement(ShowStatement* node) = 0;
    virtual void visitAssignmentStatement(AssignmentStatement* node) = 0;
}; 