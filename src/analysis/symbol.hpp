#pragma once

#include "type.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace gehu {

class Symbol {
public:
    Symbol(std::string name, std::shared_ptr<Type> type) 
        : name_(std::move(name)), type_(std::move(type)) {}
    
    // Add a constructor that takes only type (for temporary symbols)
    explicit Symbol(std::shared_ptr<Type> type) 
        : name_(""), type_(std::move(type)) {}
    
    virtual ~Symbol() = default;
    
    const std::string& name() const { return name_; }
    const std::shared_ptr<Type>& type() const { return type_; }

private:
    std::string name_;
    std::shared_ptr<Type> type_;
};

class VariableSymbol : public Symbol {
public:
    VariableSymbol(std::string name, std::shared_ptr<Type> type)
        : Symbol(std::move(name), std::move(type)) {}
};

class FunctionSymbol : public Symbol {
public:
    FunctionSymbol(std::string name, 
                  std::vector<std::shared_ptr<Type>> params,
                  std::shared_ptr<Type> return_type)
        : Symbol(std::move(name), 
                std::make_shared<FunctionType>(std::move(params), return_type)) {}
    
    const std::vector<std::shared_ptr<Type>>& params() const {
        return std::static_pointer_cast<FunctionType>(type())->params();
    }
    
    const std::shared_ptr<Type>& returnType() const {
        return std::static_pointer_cast<FunctionType>(type())->returnType();
    }
};

} // namespace gehu 