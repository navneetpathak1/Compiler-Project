#include "semantic_visitor.hpp"
#include <sstream>

namespace gehu {

void SemanticVisitor::enterScope() {
    scopes.emplace_back();
}

void SemanticVisitor::exitScope() {
    scopes.pop_back();
}

void SemanticVisitor::declare(const std::string& name, TypePtr type) {
    if (scopes.empty()) {
        throw SemanticError("No active scope");
    }
    
    if (scopes.back().count(name) > 0) {
        throw SemanticError("Variable '" + name + "' already declared in this scope");
    }
    
    scopes.back()[name] = std::move(type);
}

TypePtr SemanticVisitor::resolve(const std::string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto jt = it->find(name);
        if (jt != it->end()) {
            return jt->second;
        }
    }
    throw SemanticError("Undefined variable '" + name + "'");
}

TypePtr SemanticVisitor::checkBinaryOp(BinaryExpr::Op op, TypePtr left, TypePtr right) {
    switch (op) {
        case BinaryExpr::Op::Add:
        case BinaryExpr::Op::Sub:
        case BinaryExpr::Op::Mul:
        case BinaryExpr::Op::Div: {
            auto* leftNum = dynamic_cast<NumberType*>(left.get());
            auto* rightNum = dynamic_cast<NumberType*>(right.get());
            
            if (!leftNum || !rightNum) {
                throw SemanticError("Arithmetic operations require numeric operands");
            }
            
            // Handle unit conversions
            if (!leftNum->unit.empty() && !rightNum->unit.empty()) {
                if (leftNum->unit != rightNum->unit) {
                    throw SemanticError("Cannot perform arithmetic on values with different units");
                }
            }
            
            // Result inherits unit from left operand
            return std::make_shared<NumberType>(leftNum->unit);
        }
        
        case BinaryExpr::Op::Equal:
        case BinaryExpr::Op::NotEqual:
        case BinaryExpr::Op::Less:
        case BinaryExpr::Op::Greater:
        case BinaryExpr::Op::LessEqual:
        case BinaryExpr::Op::GreaterEqual: {
            if (!left->isAssignableFrom(*right) && !right->isAssignableFrom(*left)) {
                throw SemanticError("Cannot compare values of different types");
            }
            return std::make_shared<BooleanType>();
        }
        
        case BinaryExpr::Op::Pipeline: {
            auto* func = dynamic_cast<FunctionType*>(right.get());
            if (!func) {
                throw SemanticError("Pipeline operator requires a function on the right side");
            }
            if (func->parameterTypes.size() != 1) {
                throw SemanticError("Pipeline operator requires a function with exactly one parameter");
            }
            if (!func->parameterTypes[0]->isAssignableFrom(*left)) {
                throw SemanticError("Pipeline operator: argument type does not match function parameter type");
            }
            return func->returnType;
        }
        
        default:
            throw SemanticError("Invalid binary operator");
    }
}

TypePtr SemanticVisitor::checkUnaryOp(UnaryExpr::Op op, TypePtr operand) {
    switch (op) {
        case UnaryExpr::Op::Negate: {
            if (!dynamic_cast<NumberType*>(operand.get())) {
                throw SemanticError("Negation requires a numeric operand");
            }
            return operand;
        }
        
        case UnaryExpr::Op::Not: {
            if (!dynamic_cast<BooleanType*>(operand.get())) {
                throw SemanticError("Logical not requires a boolean operand");
            }
            return operand;
        }
        
        default:
            throw SemanticError("Invalid unary operator");
    }
}

TypePtr SemanticVisitor::visitBinaryExpr(BinaryExpr& expr) {
    TypePtr left = expr.left->accept(*this);
    TypePtr right = expr.right->accept(*this);
    return checkBinaryOp(expr.op, left, right);
}

TypePtr SemanticVisitor::visitUnaryExpr(UnaryExpr& expr) {
    TypePtr operand = expr.right->accept(*this);
    return checkUnaryOp(expr.op, operand);
}

TypePtr SemanticVisitor::visitLiteralExpr(LiteralExpr& expr) {
    if (std::holds_alternative<double>(expr.value)) {
        return std::make_shared<NumberType>(expr.unit);
    } else {
        return std::make_shared<StringType>();
    }
}

TypePtr SemanticVisitor::visitVarExpr(VarExpr& expr) {
    return resolve(expr.name);
}

TypePtr SemanticVisitor::visitAssignExpr(AssignExpr& expr) {
    TypePtr valueType = expr.value->accept(*this);
    TypePtr varType = resolve(expr.name);
    
    if (!varType->isAssignableFrom(*valueType)) {
        throw SemanticError("Cannot assign value of type " + valueType->toString() +
                          " to variable of type " + varType->toString());
    }
    
    return valueType;
}

TypePtr SemanticVisitor::visitCallExpr(CallExpr& expr) {
    TypePtr calleeType = expr.callee->accept(*this);
    auto* funcType = dynamic_cast<FunctionType*>(calleeType.get());
    
    if (!funcType) {
        throw SemanticError("Can only call functions");
    }
    
    if (expr.arguments.size() != funcType->parameterTypes.size()) {
        throw SemanticError("Function call has wrong number of arguments");
    }
    
    for (size_t i = 0; i < expr.arguments.size(); ++i) {
        TypePtr argType = expr.arguments[i]->accept(*this);
        if (!funcType->parameterTypes[i]->isAssignableFrom(*argType)) {
            throw SemanticError("Argument " + std::to_string(i + 1) + " has wrong type");
        }
    }
    
    return funcType->returnType;
}

void SemanticVisitor::visitExprStmt(ExprStmt& stmt) {
    stmt.expression->accept(*this);
}

void SemanticVisitor::visitLetStmt(LetStmt& stmt) {
    TypePtr initType = stmt.initializer->accept(*this);
    
    if (stmt.type) {
        if (!stmt.type->isAssignableFrom(*initType)) {
            throw SemanticError("Cannot initialize variable of type " + stmt.type->toString() +
                              " with value of type " + initType->toString());
        }
    } else {
        stmt.type = initType;
    }
    
    declare(stmt.name, stmt.type);
}

void SemanticVisitor::visitShowStmt(ShowStmt& stmt) {
    stmt.expression->accept(*this);
}

void SemanticVisitor::visitBlockStmt(BlockStmt& stmt) {
    enterScope();
    for (auto& s : stmt.statements) {
        s->accept(*this);
    }
    exitScope();
}

void SemanticVisitor::visitIfStmt(IfStmt& stmt) {
    TypePtr condType = stmt.condition->accept(*this);
    if (!dynamic_cast<BooleanType*>(condType.get())) {
        throw SemanticError("If condition must be a boolean");
    }
    
    stmt.thenBranch->accept(*this);
    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }
}

void SemanticVisitor::visitWhileStmt(WhileStmt& stmt) {
    TypePtr condType = stmt.condition->accept(*this);
    if (!dynamic_cast<BooleanType*>(condType.get())) {
        throw SemanticError("While condition must be a boolean");
    }
    
    stmt.body->accept(*this);
}

void SemanticVisitor::visitForStmt(ForStmt& stmt) {
    TypePtr iterType = stmt.iterator->accept(*this);
    // TODO: Add support for iterable types
    
    enterScope();
    declare(stmt.variable, std::make_shared<NumberType>());  // Default to number for now
    stmt.body->accept(*this);
    exitScope();
}

void SemanticVisitor::visitMatchStmt(MatchStmt& stmt) {
    TypePtr valueType = stmt.value->accept(*this);
    
    for (const auto& [pattern, body] : stmt.cases) {
        TypePtr patternType = pattern->accept(*this);
        if (!valueType->isAssignableFrom(*patternType)) {
            throw SemanticError("Pattern type does not match value type");
        }
        body->accept(*this);
    }
    
    if (stmt.defaultCase) {
        stmt.defaultCase->accept(*this);
    }
}

void SemanticVisitor::visitSpawnStmt(SpawnStmt& stmt) {
    stmt.statement->accept(*this);
}

void SemanticVisitor::visitWaitStmt(WaitStmt& stmt) {
    TypePtr exprType = stmt.expression->accept(*this);
    // TODO: Add support for future/promise types
}

void SemanticVisitor::visitReturnStmt(ReturnStmt& stmt) {
    if (!currentReturnType) {
        throw SemanticError("Cannot return outside of a function");
    }
    
    if (stmt.value) {
        TypePtr valueType = stmt.value->accept(*this);
        if (!currentReturnType->isAssignableFrom(*valueType)) {
            throw SemanticError("Return value type does not match function return type");
        }
    } else if (!dynamic_cast<NumberType*>(currentReturnType.get())) {
        throw SemanticError("Function must return a value");
    }
}

void SemanticVisitor::visitProgram(Program& program) {
    for (auto& stmt : program.statements) {
        stmt->accept(*this);
    }
}

} // namespace gehu 