#pragma once
#include "../parser/ast.hpp"
#include "type.hpp"
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace gehu {

class SemanticError : public std::runtime_error {
public:
    explicit SemanticError(const std::string& msg) : std::runtime_error(msg) {}
};

class SemanticVisitor : public ExprVisitor, public StmtVisitor {
private:
    TypeRegistry& typeRegistry;
    std::vector<std::unordered_map<std::string, TypePtr>> scopes;
    TypePtr currentReturnType;
    
    void enterScope();
    void exitScope();
    void declare(const std::string& name, TypePtr type);
    TypePtr resolve(const std::string& name) const;
    TypePtr checkBinaryOp(BinaryExpr::Op op, TypePtr left, TypePtr right);
    TypePtr checkUnaryOp(UnaryExpr::Op op, TypePtr operand);
    
public:
    explicit SemanticVisitor(TypeRegistry& registry) : typeRegistry(registry) {
        enterScope();  // Global scope
    }
    
    // Expression visitors
    TypePtr visitBinaryExpr(BinaryExpr& expr);
    TypePtr visitUnaryExpr(UnaryExpr& expr);
    TypePtr visitLiteralExpr(LiteralExpr& expr);
    TypePtr visitVarExpr(VarExpr& expr);
    TypePtr visitAssignExpr(AssignExpr& expr);
    TypePtr visitCallExpr(CallExpr& expr);
    
    // Statement visitors
    void visitExprStmt(ExprStmt& stmt);
    void visitLetStmt(LetStmt& stmt);
    void visitShowStmt(ShowStmt& stmt);
    void visitBlockStmt(BlockStmt& stmt);
    void visitIfStmt(IfStmt& stmt);
    void visitWhileStmt(WhileStmt& stmt);
    void visitForStmt(ForStmt& stmt);
    void visitMatchStmt(MatchStmt& stmt);
    void visitSpawnStmt(SpawnStmt& stmt);
    void visitWaitStmt(WaitStmt& stmt);
    void visitReturnStmt(ReturnStmt& stmt);
    
    // Program visitor
    void visitProgram(Program& program);
};

} // namespace gehu 