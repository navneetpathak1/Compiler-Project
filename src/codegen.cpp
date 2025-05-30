#include "ast.hpp"
#include "codegen.hpp"
#include "errors.hpp"
#include <llvm/IR/Verifier.h> // verify the LLVM IR
#include <llvm/Support/TargetSelect.h> // select the target
#include <llvm/ExecutionEngine/ExecutionEngine.h> // execute the LLVM IR
#include <llvm/ExecutionEngine/GenericValue.h> // store the LLVM generic value
#include <llvm/Support/raw_ostream.h> // store the LLVM raw ostream
#include <iostream> // for input and output
#include <llvm/ExecutionEngine/SectionMemoryManager.h> // store the LLVM section memory manager

//CodeGenerator class constructor
CodeGenerator::CodeGenerator() {
    std::cout << "[CodeGen] Initializing LLVM context..." << std::endl;
    context = std::make_unique<llvm::LLVMContext>();
    if (!context) {
        throw CodeGenError("Failed to create LLVM context", 0, 0);
    }
    
    std::cout << "[CodeGen] Creating module..." << std::endl;
    module = std::make_unique<llvm::Module>("gehu", *context);
    if (!module) {
        throw CodeGenError("Failed to create LLVM module", 0, 0);
    }
    
    std::cout << "[CodeGen] Creating IR builder..." << std::endl;
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    if (!builder) {
        throw CodeGenError("Failed to create IR builder", 0, 0);
    }
    
    std::cout << "[CodeGen] Creating printf function..." << std::endl;
    createPrintfFunction();
}

void CodeGenerator::createPrintfFunction() {
    // printf(const char*, ...)
    std::vector<llvm::Type*> printfArgs;
    printfArgs.push_back(llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0));
    llvm::FunctionType* printfType = llvm::FunctionType::get(
        builder->getInt32Ty(),
        printfArgs,
        true // variadic
    );
    printfFunction = llvm::Function::Create(
        printfType,
        llvm::Function::ExternalLinkage,
        "printf",
        module.get()
    );
}

void CodeGenerator::generate(Program* program) {
    if (!program) {
        throw CodeGenError("Null program pointer", 0, 0);
    }
    
    std::cout << "[CodeGen] Generating main function..." << std::endl;
    llvm::FunctionType* mainType = llvm::FunctionType::get(
        builder->getInt32Ty(),
        false
    );
    
    if (!mainType) {
        throw CodeGenError("Failed to create main function type", 0, 0);
    }
    
    llvm::Function* mainFunction = llvm::Function::Create(
        mainType,
        llvm::Function::ExternalLinkage,
        "main",
        module.get()
    );
    
    if (!mainFunction) {
        throw CodeGenError("Failed to create main function", 0, 0);
    }
    
    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context, "entry", mainFunction);
    if (!entry) {
        throw CodeGenError("Failed to create entry block", 0, 0);
    }
    
    builder->SetInsertPoint(entry);
    
    for (const auto& statement : program->statements) {
        if (!statement) {
            throw CodeGenError("Null statement pointer", 0, 0);
        }
        std::cout << "[CodeGen] Visiting top-level statement..." << std::endl;
        statement->accept(*this);
    }
    
    builder->CreateRet(builder->getInt32(0));
    
    std::string error;
    llvm::raw_string_ostream errorStream(error);
    if (llvm::verifyModule(*module, &errorStream)) {
        std::cerr << "[CodeGen] Module verification failed: " << error << std::endl;
        throw CodeGenError("Module verification failed: " + error, 0, 0);
    }
    std::cout << "[CodeGen] Module verified successfully." << std::endl;

    // Write the generated LLVM IR to a file for debugging
    std::error_code EC;
    llvm::raw_fd_ostream out("output.ll", EC);
    if (EC) {
        throw CodeGenError("Failed to open output file: " + EC.message(), 0, 0);
    }
    module->print(out, nullptr);
    out.flush();
    std::cout << "[CodeGen] Generated LLVM IR written to output.ll" << std::endl;
}

void CodeGenerator::visitStringLiteral(StringLiteral* node) {
    std::cout << "[CodeGen] StringLiteral: " << node->value << std::endl;
    currentValue = builder->CreateGlobalStringPtr(node->value);
}

void CodeGenerator::visitNumberLiteral(NumberLiteral* node) {
    std::cout << "[CodeGen] NumberLiteral: " << node->value << std::endl;
    currentValue = builder->getInt32(node->value);
}

void CodeGenerator::visitIdentifier(Identifier* node) {
    std::cout << "[CodeGen] Identifier: " << node->name << std::endl;
    if (variables.find(node->name) == variables.end()) {
        std::cerr << "[CodeGen] Undefined variable: " << node->name << std::endl;
        throw CodeGenError("Undefined variable: " + node->name, 0, 0);
    }
    currentValue = builder->CreateLoad(builder->getInt32Ty(), variables[node->name]);
}
// for binary expression
void CodeGenerator::visitBinaryExpression(BinaryExpression* node) {
    std::cout << "[CodeGen] BinaryExpression: op=" << static_cast<int>(node->op) << std::endl;
    node->left->accept(*this);
    llvm::Value* left = currentValue;
    node->right->accept(*this);
    llvm::Value* right = currentValue;
    switch (node->op) {
        case BinaryOperator::ADD:
            currentValue = builder->CreateAdd(left, right);
            break;
        case BinaryOperator::SUBTRACT:
            currentValue = builder->CreateSub(left, right);
            break;
        case BinaryOperator::MULTIPLY:
            currentValue = builder->CreateMul(left, right);
            break;
        case BinaryOperator::DIVIDE:
            currentValue = builder->CreateSDiv(left, right);
            break;
        case BinaryOperator::GREATER_THAN:
            currentValue = builder->CreateICmpSGT(left, right);
            break;
        case BinaryOperator::LESS_THAN:
            currentValue = builder->CreateICmpSLT(left, right);
            break;
        case BinaryOperator::GREATER_EQUAL:
            currentValue = builder->CreateICmpSGE(left, right);
            break;
        case BinaryOperator::LESS_EQUAL:
            currentValue = builder->CreateICmpSLE(left, right);
            break;
        case BinaryOperator::EQUAL_EQUAL:
            currentValue = builder->CreateICmpEQ(left, right);
            break;
        case BinaryOperator::NOT_EQUAL:
            currentValue = builder->CreateICmpNE(left, right);
            break;
    }
}
// for block
void CodeGenerator::visitBlock(Block* node) {
    std::cout << "[CodeGen] Entering block with " << node->statements.size() << " statements." << std::endl;
    for (const auto& statement : node->statements) {
        statement->accept(*this);
    }
    std::cout << "[CodeGen] Exiting block." << std::endl;
}
// for if statement
void CodeGenerator::visitIfStatement(IfStatement* node) {
    std::cout << "[CodeGen] IfStatement: Generating condition..." << std::endl;
    node->condition->accept(*this);
    llvm::Value* condition = currentValue;
    llvm::Function* function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(*context, "then", function);
    llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(*context, "else", function);
    llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(*context, "ifcont", function);
    builder->CreateCondBr(condition, thenBlock, elseBlock);
    builder->SetInsertPoint(thenBlock);
    std::cout << "[CodeGen] IfStatement: Generating then block..." << std::endl;
    node->thenBlock->accept(*this);
    builder->CreateBr(mergeBlock);
    builder->SetInsertPoint(elseBlock);
    if (node->elseBlock) {
        std::cout << "[CodeGen] IfStatement: Generating else block..." << std::endl;
        node->elseBlock->accept(*this);
    }
    builder->CreateBr(mergeBlock);
    builder->SetInsertPoint(mergeBlock);
    std::cout << "[CodeGen] IfStatement: Done." << std::endl;
}
// for variable declaration
void CodeGenerator::visitVariableDeclaration(VariableDeclaration* node) {
    std::cout << "[CodeGen] VariableDeclaration: " << node->name << std::endl;
    node->value->accept(*this);
    StringLiteral* strLit = dynamic_cast<StringLiteral*>(node->value.get());
    if (strLit) {
        // For string literals, store the global string pointer directly
        llvm::Value* strPtr = builder->CreateGlobalStringPtr(strLit->value);
        llvm::AllocaInst* alloca = builder->CreateAlloca(strPtr->getType(), nullptr, node->name);
        builder->CreateStore(strPtr, alloca);
        variables[node->name] = alloca;
    } else {
        // For non-string literals (e.g., numbers), allocate an integer
        llvm::AllocaInst* alloca = builder->CreateAlloca(builder->getInt32Ty(), nullptr, node->name);
        builder->CreateStore(currentValue, alloca);
        variables[node->name] = alloca;
    }
}
// for show statement
void CodeGenerator::visitShowStatement(ShowStatement* node) {
    std::cout << "[CodeGen] ShowStatement" << std::endl;
    StringLiteral* strLit = dynamic_cast<StringLiteral*>(node->expression.get());
    NumberLiteral* numLit = dynamic_cast<NumberLiteral*>(node->expression.get());
    Identifier* ident = dynamic_cast<Identifier*>(node->expression.get());
    if (strLit) {
        // Print string literal directly
        llvm::Value* formatStr = builder->CreateGlobalStringPtr("%s\n");
        llvm::Value* str = builder->CreateGlobalStringPtr(strLit->value);
        std::vector<llvm::Value*> args = {formatStr, str};
        builder->CreateCall(printfFunction, args);
    } else if (numLit) {
        // Print number
        llvm::Value* formatStr = builder->CreateGlobalStringPtr("%d\n");
        llvm::Value* num = builder->getInt32(numLit->value);
        std::vector<llvm::Value*> args = {formatStr, num};
        builder->CreateCall(printfFunction, args);
    } else if (ident) {
        if (variables.find(ident->name) == variables.end()) {
            throw CodeGenError("Undefined variable: " + ident->name, 0, 0);
        }
        llvm::Value* varAlloca = variables[ident->name];
        llvm::AllocaInst* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(varAlloca);
        if (!allocaInst) {
            throw CodeGenError("Variable is not an alloca instruction: " + ident->name, 0, 0);
        }
        llvm::Type* varType = allocaInst->getAllocatedType();
        std::cout << "[CodeGen] ShowStatement: Variable " << ident->name << " has type: ";
        varType->print(llvm::errs());
        std::cout << std::endl;
        
        if (varType->isIntegerTy(32)) {
            // Print integer variable
            llvm::Value* formatStr = builder->CreateGlobalStringPtr("%d\n");
            llvm::Value* val = builder->CreateLoad(builder->getInt32Ty(), varAlloca);
            std::vector<llvm::Value*> args = {formatStr, val};
            builder->CreateCall(printfFunction, args);
        } else if (varType->isPointerTy()) {
            // Print string variable
            llvm::Value* formatStr = builder->CreateGlobalStringPtr("%s\n");
            llvm::Value* val = builder->CreateLoad(varType, varAlloca);
            std::vector<llvm::Value*> args = {formatStr, val};
            builder->CreateCall(printfFunction, args);
        } else {
            throw CodeGenError("Unsupported variable type in show statement: " + ident->name, 0, 0);
        }
    } else {
        throw CodeGenError("Unsupported expression in show statement", 0, 0);
    }
}
// for assignment statement 
void CodeGenerator::visitAssignmentStatement(AssignmentStatement* node) {
    if (variables.find(node->name) == variables.end()) {
        throw CodeGenError("Assignment to undeclared variable: " + node->name, 0, 0);
    }
    node->value->accept(*this);
    builder->CreateStore(currentValue, variables[node->name]);
}
// for run  
void CodeGenerator::run() {
    std::cout << "[CodeGen] Initializing native target..." << std::endl;
    if (llvm::InitializeNativeTarget()) {
        throw CodeGenError("Failed to initialize native target", 0, 0);
    }
    
    std::cout << "[CodeGen] Initializing native target asm printer..." << std::endl;
    if (llvm::InitializeNativeTargetAsmPrinter()) {
        throw CodeGenError("Failed to initialize native target asm printer", 0, 0);
    }
    
    std::cout << "[CodeGen] Initializing native target asm parser..." << std::endl;
    if (llvm::InitializeNativeTargetAsmParser()) {
        throw CodeGenError("Failed to initialize native target asm parser", 0, 0);
    }
    
    std::cout << "[CodeGen] Creating execution engine..." << std::endl;
    std::string error;
    // Create a temporary unique_ptr for the module
    std::unique_ptr<llvm::Module> tempModule = std::move(module);
    llvm::EngineBuilder builder(std::move(tempModule));
    builder.setErrorStr(&error);
    builder.setVerifyModules(true);
    builder.setOptLevel(llvm::CodeGenOptLevel::None);  // Disable optimizations for debugging
    
    llvm::ExecutionEngine* engine = builder.create();
    if (!engine) {
        throw CodeGenError("Failed to create execution engine: " + error, 0, 0);
    }

    // Register printf symbol for JIT
    engine->addGlobalMapping("printf", (uint64_t)&printf);

    std::cout << "[CodeGen] Getting main function pointer..." << std::endl;
    llvm::Function* mainFunc = engine->FindFunctionNamed("main");
    if (!mainFunc) {
        delete engine;
        throw CodeGenError("Failed to find main function", 0, 0);
    }
    
    std::cout << "[CodeGen] Executing main..." << std::endl;
    try {
        // Use runFunction instead of getPointerToFunction
        std::vector<llvm::GenericValue> noargs;
        engine->runFunction(mainFunc, noargs);
    } catch (const std::exception& e) {
        delete engine;
        throw CodeGenError("Exception during execution: " + std::string(e.what()), 0, 0);
    } catch (...) {
        delete engine;
        throw CodeGenError("Unknown exception during execution", 0, 0);
    }
    
    delete engine; // delete the execution engine
} 