#include "codegen_visitor.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <stdexcept>

namespace gehu {

CodegenVisitor::CodegenVisitor() {
    context_ = std::make_unique<llvm::LLVMContext>();
    module_ = std::make_unique<llvm::Module>("gehu", *context_);
    builder_ = std::make_unique<llvm::IRBuilder<>>(*context_);
}

void CodegenVisitor::visitProgram(Program& program) {
    for (auto& stmt : program.statements) {
        stmt->accept(*this);
    }
}

void CodegenVisitor::visitBlock(Block& block) {
    for (auto& stmt : block.statements) {
        stmt->accept(*this);
    }
}

void CodegenVisitor::visitVarDecl(VarDecl& varDecl) {
    llvm::Value* init = nullptr;
    if (varDecl.initializer) {
        init = varDecl.initializer->accept(*this);
    } else {
        init = llvm::ConstantFP::get(*context_, llvm::APFloat(0.0));
    }

    llvm::AllocaInst* alloca = builder_->CreateAlloca(llvm::Type::getDoubleTy(*context_), nullptr, varDecl.name);
    builder_->CreateStore(init, alloca);
    named_values_[varDecl.name] = alloca;
}

void CodegenVisitor::visitFuncDecl(FuncDecl& funcDecl) {
    std::vector<llvm::Type*> param_types(funcDecl.params.size(), llvm::Type::getDoubleTy(*context_));
    llvm::FunctionType* func_type = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*context_), param_types, false);

    llvm::Function* function = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage, funcDecl.name, module_.get());

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context_, "entry", function);
    builder_->SetInsertPoint(entry);

    named_values_.clear();
    for (size_t i = 0; i < funcDecl.params.size(); ++i) {
        llvm::AllocaInst* alloca = builder_->CreateAlloca(
            llvm::Type::getDoubleTy(*context_), nullptr, funcDecl.params[i]);
        builder_->CreateStore(function->getArg(i), alloca);
        named_values_[funcDecl.params[i]] = alloca;
    }

    funcDecl.body->accept(*this);

    if (!builder_->GetInsertBlock()->getTerminator()) {
        builder_->CreateRet(llvm::ConstantFP::get(*context_, llvm::APFloat(0.0)));
    }

    llvm::verifyFunction(*function);
}

void CodegenVisitor::visitIfStmt(IfStmt& ifStmt) {
    llvm::Value* cond = ifStmt.condition->accept(*this);
    cond = builder_->CreateFCmpONE(cond, llvm::ConstantFP::get(*context_, llvm::APFloat(0.0)), "ifcond");

    llvm::Function* function = builder_->GetInsertBlock()->getParent();
    llvm::BasicBlock* then_bb = llvm::BasicBlock::Create(*context_, "then", function);
    llvm::BasicBlock* else_bb = llvm::BasicBlock::Create(*context_, "else");
    llvm::BasicBlock* merge_bb = llvm::BasicBlock::Create(*context_, "ifcont");

    builder_->CreateCondBr(cond, then_bb, else_bb);

    builder_->SetInsertPoint(then_bb);
    ifStmt.thenBranch->accept(*this);
    builder_->CreateBr(merge_bb);

    function->getBasicBlockList().push_back(else_bb);
    builder_->SetInsertPoint(else_bb);
    if (ifStmt.elseBranch) {
        ifStmt.elseBranch->accept(*this);
    }
    builder_->CreateBr(merge_bb);

    function->getBasicBlockList().push_back(merge_bb);
    builder_->SetInsertPoint(merge_bb);
}

void CodegenVisitor::visitWhileStmt(WhileStmt& whileStmt) {
    llvm::Function* function = builder_->GetInsertBlock()->getParent();
    llvm::BasicBlock* loop_bb = llvm::BasicBlock::Create(*context_, "loop", function);
    llvm::BasicBlock* after_bb = llvm::BasicBlock::Create(*context_, "afterloop");

    builder_->CreateBr(loop_bb);
    builder_->SetInsertPoint(loop_bb);

    llvm::Value* cond = whileStmt.condition->accept(*this);
    cond = builder_->CreateFCmpONE(cond, llvm::ConstantFP::get(*context_, llvm::APFloat(0.0)), "whilecond");

    llvm::BasicBlock* body_bb = llvm::BasicBlock::Create(*context_, "body", function);
    builder_->CreateCondBr(cond, body_bb, after_bb);

    builder_->SetInsertPoint(body_bb);
    whileStmt.body->accept(*this);
    builder_->CreateBr(loop_bb);

    function->getBasicBlockList().push_back(after_bb);
    builder_->SetInsertPoint(after_bb);
}

void CodegenVisitor::visitForStmt(ForStmt& forStmt) {
    llvm::Function* function = builder_->GetInsertBlock()->getParent();
    llvm::BasicBlock* loop_bb = llvm::BasicBlock::Create(*context_, "loop", function);
    llvm::BasicBlock* after_bb = llvm::BasicBlock::Create(*context_, "afterloop");

    if (forStmt.initializer) {
        forStmt.initializer->accept(*this);
    }

    builder_->CreateBr(loop_bb);
    builder_->SetInsertPoint(loop_bb);

    llvm::Value* cond = nullptr;
    if (forStmt.condition) {
        cond = forStmt.condition->accept(*this);
        cond = builder_->CreateFCmpONE(cond, llvm::ConstantFP::get(*context_, llvm::APFloat(0.0)), "forcond");
    } else {
        cond = llvm::ConstantInt::getTrue(*context_);
    }

    llvm::BasicBlock* body_bb = llvm::BasicBlock::Create(*context_, "body", function);
    builder_->CreateCondBr(cond, body_bb, after_bb);

    builder_->SetInsertPoint(body_bb);
    forStmt.body->accept(*this);

    if (forStmt.increment) {
        forStmt.increment->accept(*this);
    }

    builder_->CreateBr(loop_bb);

    function->getBasicBlockList().push_back(after_bb);
    builder_->SetInsertPoint(after_bb);
}

void CodegenVisitor::visitReturnStmt(ReturnStmt& returnStmt) {
    llvm::Value* ret_val = nullptr;
    if (returnStmt.value) {
        ret_val = returnStmt.value->accept(*this);
    } else {
        ret_val = llvm::ConstantFP::get(*context_, llvm::APFloat(0.0));
    }
    builder_->CreateRet(ret_val);
}

void CodegenVisitor::visitExprStmt(ExprStmt& exprStmt) {
    exprStmt.expression->accept(*this);
}

llvm::Value* CodegenVisitor::visitBinaryExpr(BinaryExpr& binaryExpr) {
    llvm::Value* left = binaryExpr.left->accept(*this);
    llvm::Value* right = binaryExpr.right->accept(*this);

    switch (binaryExpr.op[0]) {
        case '+':
            return builder_->CreateFAdd(left, right, "addtmp");
        case '-':
            return builder_->CreateFSub(left, right, "subtmp");
        case '*':
            return builder_->CreateFMul(left, right, "multmp");
        case '/':
            return builder_->CreateFDiv(left, right, "divtmp");
        case '=':
            return builder_->CreateFCmpOEQ(left, right, "eqtmp");
        case '!':
            return builder_->CreateFCmpONE(left, right, "netmp");
        default:
            throw std::runtime_error("Invalid binary operator");
    }
}

llvm::Value* CodegenVisitor::visitUnaryExpr(UnaryExpr& unaryExpr) {
    llvm::Value* operand = unaryExpr.right->accept(*this);

    switch (unaryExpr.op[0]) {
        case '-':
            return builder_->CreateFNeg(operand, "negtmp");
        case '!':
            return builder_->CreateFCmpOEQ(operand, 
                llvm::ConstantFP::get(*context_, llvm::APFloat(0.0)), "nottmp");
        default:
            throw std::runtime_error("Invalid unary operator");
    }
}

llvm::Value* CodegenVisitor::visitLiteralExpr(LiteralExpr& literalExpr) {
    if (std::holds_alternative<double>(literalExpr.value)) {
        return llvm::ConstantFP::get(*context_, llvm::APFloat(std::get<double>(literalExpr.value)));
    } else if (std::holds_alternative<std::string>(literalExpr.value)) {
        return builder_->CreateGlobalStringPtr(std::get<std::string>(literalExpr.value));
    } else if (std::holds_alternative<bool>(literalExpr.value)) {
        return llvm::ConstantInt::get(*context_, llvm::APInt(1, std::get<bool>(literalExpr.value)));
    } else {
        return llvm::ConstantPointerNull::get(llvm::Type::getInt8PtrTy(*context_));
    }
}

llvm::Value* CodegenVisitor::visitVarExpr(VarExpr& varExpr) {
    llvm::Value* var = named_values_[varExpr.name];
    if (!var) {
        throw std::runtime_error("Unknown variable name");
    }
    return builder_->CreateLoad(llvm::Type::getDoubleTy(*context_), var, varExpr.name.c_str());
}

llvm::Value* CodegenVisitor::visitAssignExpr(AssignExpr& assignExpr) {
    llvm::Value* value = assignExpr.value->accept(*this);
    llvm::Value* var = named_values_[assignExpr.name];
    if (!var) {
        throw std::runtime_error("Unknown variable name");
    }
    builder_->CreateStore(value, var);
    return value;
}

llvm::Value* CodegenVisitor::visitCallExpr(CallExpr& callExpr) {
    llvm::Value* callee = callExpr.callee->accept(*this);
    if (!callee->getType()->isPointerTy()) {
        throw std::runtime_error("Expected function pointer");
    }

    std::vector<llvm::Value*> args;
    for (auto& arg : callExpr.arguments) {
        args.push_back(arg->accept(*this));
    }

    return builder_->CreateCall(callee, args, "calltmp");
}

llvm::Value* CodegenVisitor::visitGetExpr(GetExpr& getExpr) {
    // TODO: Implement object property access
    return llvm::ConstantFP::get(*context_, llvm::APFloat(0.0));
}

llvm::Value* CodegenVisitor::visitSetExpr(SetExpr& setExpr) {
    // TODO: Implement object property assignment
    return setExpr.value->accept(*this);
}

llvm::Value* CodegenVisitor::visitThisExpr(ThisExpr& thisExpr) {
    // TODO: Implement this expression
    return llvm::ConstantFP::get(*context_, llvm::APFloat(0.0));
}

llvm::Value* CodegenVisitor::visitSuperExpr(SuperExpr& superExpr) {
    // TODO: Implement super expression
    return llvm::ConstantFP::get(*context_, llvm::APFloat(0.0));
}

} // namespace gehu 