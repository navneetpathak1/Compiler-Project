#pragma once

#include "ast_forward.hpp"
#include "ast_visitor.hpp"
#include <vector>
#include <memory>
#include <string>

enum class BinaryOperator {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    GREATER_THAN,
    LESS_THAN,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL
};

class Expression {
public:
    virtual ~Expression() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class Statement {
public:
    virtual ~Statement() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class StringLiteral : public Expression {
public:
    std::string value;
    StringLiteral(const std::string& value) : value(value) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitStringLiteral(this);
    }
};

class NumberLiteral : public Expression {
public:
    int value;
    NumberLiteral(int value) : value(value) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitNumberLiteral(this);
    }
};

class Identifier : public Expression {
public:
    std::string name;
    Identifier(const std::string& name) : name(name) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitIdentifier(this);
    }
};

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    BinaryOperator op;
    std::unique_ptr<Expression> right;
    BinaryExpression(std::unique_ptr<Expression> left, BinaryOperator op, std::unique_ptr<Expression> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitBinaryExpression(this);
    }
};

class Block : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    Block(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(statements)) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitBlock(this);
    }
};

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> thenBlock;
    std::unique_ptr<Block> elseBlock;
    IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Block> thenBlock, std::unique_ptr<Block> elseBlock)
        : condition(std::move(condition)), thenBlock(std::move(thenBlock)), elseBlock(std::move(elseBlock)) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitIfStatement(this);
    }
};

class VariableDeclaration : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
    VariableDeclaration(const std::string& name, std::unique_ptr<Expression> value)
        : name(name), value(std::move(value)) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitVariableDeclaration(this);
    }
};

class ShowStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    ShowStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitShowStatement(this);
    }
};

class AssignmentStatement : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> value;
    AssignmentStatement(const std::string& name, std::unique_ptr<Expression> value)
        : name(name), value(std::move(value)) {}
    void accept(ASTVisitor& visitor) override {
        visitor.visitAssignmentStatement(this);
    }
};

class Program {
public:
    std::vector<std::unique_ptr<Statement>> statements;
}; 