#pragma once

#include "../ast.hpp"
#include "../lexer.hpp"
#include "type.hpp"
#include "symbol.hpp"
#include "scope.hpp"
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace gehu {

// Forward declarations
class Program;
class Block;
class LetStmt;
class FunctionStmt;
class ShowStmt;
class ExpressionStmt;
class BlockStmt;
class SpawnStmt;
class WaitStmt;
class VarDecl;
class FuncDecl;
class IfStmt;
class WhileStmt;
class ForStmt;
class ReturnStmt;
class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class VariableExpr;
class PipelineExpr;
class UnitExpr;
class PatternMatchExpr;
class AssignExpr;
class GetExpr;
class SetExpr;
class ThisExpr;
class SuperExpr;
class CallExpr;

class SemanticVisitor {
public:
    virtual ~SemanticVisitor() = default;

    // Program
    virtual void visitProgram(Program& program) = 0;

    // Statements
    virtual void visitBlock(Block& block) = 0;
    virtual void visitLetStmt(LetStmt& stmt) = 0;
    virtual void visitFunctionStmt(FunctionStmt& stmt) = 0;
    virtual void visitShowStmt(ShowStmt& stmt) = 0;
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
    virtual void visitSpawnStmt(SpawnStmt& stmt) = 0;
    virtual void visitWaitStmt(WaitStmt& stmt) = 0;
    virtual void visitVarDecl(VarDecl& stmt) = 0;
    virtual void visitFuncDecl(FuncDecl& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
    virtual void visitForStmt(ForStmt& stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt& stmt) = 0;

    // Expressions
    virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual void visitUnaryExpr(UnaryExpr& expr) = 0;
    virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual void visitVariableExpr(VariableExpr& expr) = 0;
    virtual void visitPipelineExpr(PipelineExpr& expr) = 0;
    virtual void visitUnitExpr(UnitExpr& expr) = 0;
    virtual void visitPatternMatchExpr(PatternMatchExpr& expr) = 0;
    virtual void visitAssignExpr(AssignExpr& expr) = 0;
    virtual void visitGetExpr(GetExpr& expr) = 0;
    virtual void visitSetExpr(SetExpr& expr) = 0;
    virtual void visitThisExpr(ThisExpr& expr) = 0;
    virtual void visitSuperExpr(SuperExpr& expr) = 0;
    virtual void visitCallExpr(CallExpr& expr) = 0;
};

} // namespace gehu 