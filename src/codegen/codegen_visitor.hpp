#pragma once

#include "ast.hpp"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace gehu {

class CodegenVisitor {
public:
    CodegenVisitor();

    // Visitor methods
    void visitProgram(Program& program);
    void visitBlock(Block& block);
    void visitVarDecl(VarDecl& varDecl);
    void visitFuncDecl(FuncDecl& funcDecl);
    void visitIfStmt(IfStmt& ifStmt);
    void visitWhileStmt(WhileStmt& whileStmt);
    void visitForStmt(ForStmt& forStmt);
    void visitReturnStmt(ReturnStmt& returnStmt);
    void visitExprStmt(ExprStmt& exprStmt);

    llvm::Value* visitBinaryExpr(BinaryExpr& binaryExpr);
    llvm::Value* visitUnaryExpr(UnaryExpr& unaryExpr);
    llvm::Value* visitLiteralExpr(LiteralExpr& literalExpr);
    llvm::Value* visitVarExpr(VarExpr& varExpr);
    llvm::Value* visitAssignExpr(AssignExpr& assignExpr);
    llvm::Value* visitCallExpr(CallExpr& callExpr);
    llvm::Value* visitGetExpr(GetExpr& getExpr);
    llvm::Value* visitSetExpr(SetExpr& setExpr);
    llvm::Value* visitThisExpr(ThisExpr& thisExpr);
    llvm::Value* visitSuperExpr(SuperExpr& superExpr);

private:
    std::unique_ptr<llvm::LLVMContext> context_;
    std::unique_ptr<llvm::Module> module_;
    std::unique_ptr<llvm::IRBuilder> builder_;
    std::unordered_map<std::string, llvm::Value*> named_values_;
};

} // namespace gehu 