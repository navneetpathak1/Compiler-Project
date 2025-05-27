#pragma once
#include "../parser/ast.hpp"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace gehu {

class CodegenVisitor {
private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unordered_map<std::string, llvm::Value*> namedValues;
    std::vector<std::unordered_map<std::string, llvm::Value*>> scopes;
    llvm::Function* currentFunction = nullptr;
    
    void enterScope();
    void exitScope();
    llvm::Value* resolve(const std::string& name);
    llvm::Type* getLLVMType(TypePtr type);
    llvm::Value* convertUnit(llvm::Value* value, const std::string& fromUnit, const std::string& toUnit);
    
public:
    CodegenVisitor();
    
    // Expression visitors
    llvm::Value* visitBinaryExpr(BinaryExpr& expr);
    llvm::Value* visitUnaryExpr(UnaryExpr& expr);
    llvm::Value* visitLiteralExpr(LiteralExpr& expr);
    llvm::Value* visitVarExpr(VarExpr& expr);
    llvm::Value* visitAssignExpr(AssignExpr& expr);
    llvm::Value* visitCallExpr(CallExpr& expr);
    
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
    
    // Runtime functions
    void declareRuntimeFunctions();
    
    // Module access
    llvm::Module* getModule() const { return module.get(); }
    void optimize();
};

} // namespace gehu 