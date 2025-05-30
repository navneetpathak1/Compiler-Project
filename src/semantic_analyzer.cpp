#include "ast.hpp"
#include "semantic_analyzer.hpp"
#include "errors.hpp"

void SemanticAnalyzer::analyze(Program* program) {
    for (const auto& statement : program->statements) {
        statement->accept(*this);
    }
}

void SemanticAnalyzer::visitStringLiteral(StringLiteral* node) {
    // String literals are always valid
}

void SemanticAnalyzer::visitNumberLiteral(NumberLiteral* node) {
    // Number literals are always valid
}

void SemanticAnalyzer::visitIdentifier(Identifier* node) {
    if (variables.find(node->name) == variables.end()) {
        throw SemanticError("Undefined variable: " + node->name, 0, 0);
    }
}

void SemanticAnalyzer::visitBinaryExpression(BinaryExpression* node) {
    node->left->accept(*this);
    node->right->accept(*this);
    
    // Check for valid comparison operations
    switch (node->op) {
        case BinaryOperator::GREATER_THAN:
        case BinaryOperator::LESS_THAN:
        case BinaryOperator::GREATER_EQUAL:
        case BinaryOperator::LESS_EQUAL:
        case BinaryOperator::EQUAL_EQUAL:
        case BinaryOperator::NOT_EQUAL:
            // Comparisons are valid between numbers
            break;
        case BinaryOperator::ADD:
        case BinaryOperator::SUBTRACT:
        case BinaryOperator::MULTIPLY:
        case BinaryOperator::DIVIDE:
            // Arithmetic operations are valid between numbers
            break;
    }
}

void SemanticAnalyzer::visitBlock(Block* node) {
    // Create a new scope for the block
    std::map<std::string, bool> oldVariables = variables;
    
    // Analyze statements in the block
    for (const auto& statement : node->statements) {
        statement->accept(*this);
    }
    
    // Restore the old scope
    variables = oldVariables;
}

void SemanticAnalyzer::visitIfStatement(IfStatement* node) {
    // Analyze the condition
    node->condition->accept(*this);
    
    // Analyze the then block
    node->thenBlock->accept(*this);
    
    // Analyze the else block if present
    if (node->elseBlock) {
        node->elseBlock->accept(*this);
    }
}

void SemanticAnalyzer::visitVariableDeclaration(VariableDeclaration* node) {
    // Check if variable is already declared
    if (variables.find(node->name) != variables.end()) {
        throw SemanticError("Variable already declared: " + node->name, 0, 0);
    }
    
    // Analyze the initializer expression
    node->value->accept(*this);
    
    // Add variable to current scope
    variables[node->name] = true; // Track declared variable
}

void SemanticAnalyzer::visitShowStatement(ShowStatement* node) {
    node->expression->accept(*this);
}

void SemanticAnalyzer::visitAssignmentStatement(AssignmentStatement* node) {
    // Check if variable is declared
    if (variables.find(node->name) == variables.end()) {
        throw SemanticError("Assignment to undeclared variable: " + node->name, 0, 0);
    }
    // Analyze the assigned value
    node->value->accept(*this);
} 
