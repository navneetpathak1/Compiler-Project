#include "codegen_visitor.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <stdexcept>

namespace gehu {

CodegenVisitor::CodegenVisitor() {
    context = std::make_unique<llvm::LLVMContext>();
    module = std::make_unique<llvm::Module>("gehu", *context);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    enterScope();
    declareRuntimeFunctions();
}

void CodegenVisitor::enterScope() {
    scopes.emplace_back();
}

void CodegenVisitor::exitScope() {
    scopes.pop_back();
}

llvm::Value* CodegenVisitor::resolve(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto jt = it->find(name);
        if (jt != it->end()) {
            return jt->second;
        }
    }
    throw std::runtime_error("Undefined variable: " + name);
}

llvm::Type* CodegenVisitor::getLLVMType(TypePtr type) {
    if (auto* num = dynamic_cast<NumberType*>(type.get())) {
        return llvm::Type::getDoubleTy(*context);
    }
    if (dynamic_cast<StringType*>(type.get())) {
        return llvm::Type::getInt8PtrTy(*context);
    }
    if (dynamic_cast<BooleanType*>(type.get())) {
        return llvm::Type::getInt1Ty(*context);
    }
    if (auto* func = dynamic_cast<FunctionType*>(type.get())) {
        std::vector<llvm::Type*> paramTypes;
        for (const auto& param : func->parameterTypes) {
            paramTypes.push_back(getLLVMType(param));
        }
        return llvm::FunctionType::get(getLLVMType(func->returnType), paramTypes, false);
    }
    throw std::runtime_error("Unsupported type");
}

llvm::Value* CodegenVisitor::convertUnit(llvm::Value* value, const std::string& fromUnit, const std::string& toUnit) {
    if (fromUnit == toUnit) return value;
    
    // Get the conversion function
    std::string funcName = "convert_" + fromUnit + "_to_" + toUnit;
    llvm::Function* convertFunc = module->getFunction(funcName);
    
    if (!convertFunc) {
        throw std::runtime_error("No conversion function found from " + fromUnit + " to " + toUnit);
    }
    
    return builder->CreateCall(convertFunc, value);
}

void CodegenVisitor::declareRuntimeFunctions() {
    // Print function
    std::vector<llvm::Type*> printArgs = {llvm::Type::getDoubleTy(*context)};
    llvm::FunctionType* printType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), printArgs, false);
    llvm::Function::Create(printType, llvm::Function::ExternalLinkage, "print", module.get());
    
    // Unit conversion functions
    auto declareConversion = [this](const std::string& from, const std::string& to) {
        std::string name = "convert_" + from + "_to_" + to;
        std::vector<llvm::Type*> args = {llvm::Type::getDoubleTy(*context)};
        llvm::FunctionType* type = llvm::FunctionType::get(
            llvm::Type::getDoubleTy(*context), args, false);
        llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, module.get());
    };
    
    // Common unit conversions
    declareConversion("km", "m");
    declareConversion("m", "km");
    declareConversion("min", "s");
    declareConversion("s", "min");
    
    // Thread functions
    std::vector<llvm::Type*> spawnArgs = {llvm::Type::getInt8PtrTy(*context)};
    llvm::FunctionType* spawnType = llvm::FunctionType::get(
        llvm::Type::getInt8PtrTy(*context), spawnArgs, false);
    llvm::Function::Create(spawnType, llvm::Function::ExternalLinkage, "spawn_thread", module.get());
    
    std::vector<llvm::Type*> waitArgs = {llvm::Type::getInt8PtrTy(*context)};
    llvm::FunctionType* waitType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), waitArgs, false);
    llvm::Function::Create(waitType, llvm::Function::ExternalLinkage, "wait_thread", module.get());
}

llvm::Value* CodegenVisitor::visitBinaryExpr(BinaryExpr& expr) {
    llvm::Value* left = expr.left->accept(*this);
    llvm::Value* right = expr.right->accept(*this);
    
    switch (expr.op) {
        case BinaryExpr::Op::Add:
            return builder->CreateFAdd(left, right, "addtmp");
        case BinaryExpr::Op::Sub:
            return builder->CreateFSub(left, right, "subtmp");
        case BinaryExpr::Op::Mul:
            return builder->CreateFMul(left, right, "multmp");
        case BinaryExpr::Op::Div:
            return builder->CreateFDiv(left, right, "divtmp");
        case BinaryExpr::Op::Equal:
            return builder->CreateFCmpOEQ(left, right, "eqtmp");
        case BinaryExpr::Op::NotEqual:
            return builder->CreateFCmpONE(left, right, "netmp");
        case BinaryExpr::Op::Less:
            return builder->CreateFCmpOLT(left, right, "lttmp");
        case BinaryExpr::Op::Greater:
            return builder->CreateFCmpOGT(left, right, "gttmp");
        case BinaryExpr::Op::LessEqual:
            return builder->CreateFCmpOLE(left, right, "letmp");
        case BinaryExpr::Op::GreaterEqual:
            return builder->CreateFCmpOGE(left, right, "getmp");
        case BinaryExpr::Op::Pipeline: {
            auto* func = llvm::cast<llvm::Function>(right);
            return builder->CreateCall(func, left, "pipetmp");
        }
        default:
            throw std::runtime_error("Invalid binary operator");
    }
}

llvm::Value* CodegenVisitor::visitUnaryExpr(UnaryExpr& expr) {
    llvm::Value* operand = expr.right->accept(*this);
    
    switch (expr.op) {
        case UnaryExpr::Op::Negate:
            return builder->CreateFNeg(operand, "negtmp");
        case UnaryExpr::Op::Not:
            return builder->CreateNot(operand, "nottmp");
        default:
            throw std::runtime_error("Invalid unary operator");
    }
}

llvm::Value* CodegenVisitor::visitLiteralExpr(LiteralExpr& expr) {
    if (std::holds_alternative<double>(expr.value)) {
        double value = std::get<double>(expr.value);
        llvm::Value* result = llvm::ConstantFP::get(*context, llvm::APFloat(value));
        
        if (!expr.unit.empty()) {
            // Convert to base unit if necessary
            result = convertUnit(result, expr.unit, "");
        }
        
        return result;
    } else {
        const std::string& str = std::get<std::string>(expr.value);
        return builder->CreateGlobalStringPtr(str);
    }
}

llvm::Value* CodegenVisitor::visitVarExpr(VarExpr& expr) {
    llvm::Value* var = resolve(expr.name);
    return builder->CreateLoad(var, expr.name);
}

llvm::Value* CodegenVisitor::visitAssignExpr(AssignExpr& expr) {
    llvm::Value* value = expr.value->accept(*this);
    llvm::Value* var = resolve(expr.name);
    builder->CreateStore(value, var);
    return value;
}

llvm::Value* CodegenVisitor::visitCallExpr(CallExpr& expr) {
    llvm::Value* callee = expr.callee->accept(*this);
    
    if (!llvm::isa<llvm::Function>(callee)) {
        throw std::runtime_error("Can only call functions");
    }
    
    llvm::Function* func = llvm::cast<llvm::Function>(callee);
    
    if (expr.arguments.size() != func->arg_size()) {
        throw std::runtime_error("Function call has wrong number of arguments");
    }
    
    std::vector<llvm::Value*> args;
    for (const auto& arg : expr.arguments) {
        args.push_back(arg->accept(*this));
    }
    
    return builder->CreateCall(func, args, "calltmp");
}

void CodegenVisitor::visitExprStmt(ExprStmt& stmt) {
    stmt.expression->accept(*this);
}

void CodegenVisitor::visitLetStmt(LetStmt& stmt) {
    llvm::Value* init = stmt.initializer->accept(*this);
    llvm::Type* type = getLLVMType(stmt.type);
    
    llvm::AllocaInst* alloca = builder->CreateAlloca(type, nullptr, stmt.name);
    builder->CreateStore(init, alloca);
    scopes.back()[stmt.name] = alloca;
}

void CodegenVisitor::visitShowStmt(ShowStmt& stmt) {
    llvm::Value* value = stmt.expression->accept(*this);
    llvm::Function* printFunc = module->getFunction("print");
    builder->CreateCall(printFunc, value);
}

void CodegenVisitor::visitBlockStmt(BlockStmt& stmt) {
    enterScope();
    for (auto& s : stmt.statements) {
        s->accept(*this);
    }
    exitScope();
}

void CodegenVisitor::visitIfStmt(IfStmt& stmt) {
    llvm::Value* cond = stmt.condition->accept(*this);
    cond = builder->CreateFCmpONE(cond, llvm::ConstantFP::get(*context, llvm::APFloat(0.0)), "ifcond");
    
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(*context, "then", function);
    llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(*context, "ifcont");
    
    builder->CreateCondBr(cond, thenBB, elseBB);
    
    builder->SetInsertPoint(thenBB);
    stmt.thenBranch->accept(*this);
    builder->CreateBr(mergeBB);
    
    function->getBasicBlockList().push_back(elseBB);
    builder->SetInsertPoint(elseBB);
    if (stmt.elseBranch) {
        stmt.elseBranch->accept(*this);
    }
    builder->CreateBr(mergeBB);
    
    function->getBasicBlockList().push_back(mergeBB);
    builder->SetInsertPoint(mergeBB);
}

void CodegenVisitor::visitWhileStmt(WhileStmt& stmt) {
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(*context, "loop", function);
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(*context, "afterloop");
    
    builder->CreateBr(loopBB);
    builder->SetInsertPoint(loopBB);
    
    llvm::Value* cond = stmt.condition->accept(*this);
    cond = builder->CreateFCmpONE(cond, llvm::ConstantFP::get(*context, llvm::APFloat(0.0)), "whilecond");
    
    llvm::BasicBlock* bodyBB = llvm::BasicBlock::Create(*context, "body", function);
    builder->CreateCondBr(cond, bodyBB, afterBB);
    
    builder->SetInsertPoint(bodyBB);
    stmt.body->accept(*this);
    builder->CreateBr(loopBB);
    
    function->getBasicBlockList().push_back(afterBB);
    builder->SetInsertPoint(afterBB);
}

void CodegenVisitor::visitForStmt(ForStmt& stmt) {
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(*context, "loop", function);
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(*context, "afterloop");
    
    llvm::Value* iterator = stmt.iterator->accept(*this);
    // TODO: Implement iterator support
    
    builder->CreateBr(loopBB);
    builder->SetInsertPoint(loopBB);
    
    enterScope();
    llvm::AllocaInst* alloca = builder->CreateAlloca(llvm::Type::getDoubleTy(*context), nullptr, stmt.variable);
    scopes.back()[stmt.variable] = alloca;
    
    stmt.body->accept(*this);
    
    exitScope();
    builder->CreateBr(loopBB);
    
    function->getBasicBlockList().push_back(afterBB);
    builder->SetInsertPoint(afterBB);
}

void CodegenVisitor::visitMatchStmt(MatchStmt& stmt) {
    llvm::Value* value = stmt.value->accept(*this);
    
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(*context, "aftermatch");
    
    for (const auto& [pattern, body] : stmt.cases) {
        llvm::Value* patternValue = pattern->accept(*this);
        llvm::Value* cond = builder->CreateFCmpOEQ(value, patternValue, "matchcond");
        
        llvm::BasicBlock* caseBB = llvm::BasicBlock::Create(*context, "case", function);
        llvm::BasicBlock* nextBB = llvm::BasicBlock::Create(*context, "nextcase");
        
        builder->CreateCondBr(cond, caseBB, nextBB);
        
        builder->SetInsertPoint(caseBB);
        body->accept(*this);
        builder->CreateBr(afterBB);
        
        function->getBasicBlockList().push_back(nextBB);
        builder->SetInsertPoint(nextBB);
    }
    
    if (stmt.defaultCase) {
        stmt.defaultCase->accept(*this);
    }
    builder->CreateBr(afterBB);
    
    function->getBasicBlockList().push_back(afterBB);
    builder->SetInsertPoint(afterBB);
}

void CodegenVisitor::visitSpawnStmt(SpawnStmt& stmt) {
    llvm::Function* spawnFunc = module->getFunction("spawn_thread");
    if (!spawnFunc) {
        throw std::runtime_error("Spawn function not found");
    }
    
    // Create a function for the spawned code
    llvm::FunctionType* funcType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context), false);
    llvm::Function* func = llvm::Function::Create(
        funcType, llvm::Function::PrivateLinkage, "spawned_func", module.get());
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context, "entry", func);
    builder->SetInsertPoint(entry);
    
    stmt.statement->accept(*this);
    builder->CreateRetVoid();
    
    builder->SetInsertPoint(builder->GetInsertBlock()->getParent()->getEntryBlock().getTerminator());
    builder->CreateCall(spawnFunc, func);
}

void CodegenVisitor::visitWaitStmt(WaitStmt& stmt) {
    llvm::Function* waitFunc = module->getFunction("wait_thread");
    if (!waitFunc) {
        throw std::runtime_error("Wait function not found");
    }
    
    llvm::Value* thread = stmt.expression->accept(*this);
    builder->CreateCall(waitFunc, thread);
}

void CodegenVisitor::visitReturnStmt(ReturnStmt& stmt) {
    if (!currentFunction) {
        throw std::runtime_error("Cannot return outside of a function");
    }
    
    if (stmt.value) {
        llvm::Value* retVal = stmt.value->accept(*this);
        builder->CreateRet(retVal);
    } else {
        builder->CreateRetVoid();
    }
}

void CodegenVisitor::visitProgram(Program& program) {
    for (auto& stmt : program.statements) {
        stmt->accept(*this);
    }
}

void CodegenVisitor::optimize() {
    llvm::FunctionPassManager fpm(module.get());
    
    // Add optimization passes
    fpm.add(llvm::createInstructionCombiningPass());
    fpm.add(llvm::createReassociatePass());
    fpm.add(llvm::createGVNPass());
    fpm.add(llvm::createCFGSimplificationPass());
    
    // Run optimizations
    for (auto& func : *module) {
        fpm.run(func);
    }
}

} // namespace gehu 