#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace gehu {

class Type {
public:
    virtual ~Type() = default;
    virtual bool isAssignableFrom(const Type& other) const = 0;
    virtual std::string toString() const = 0;
};

class NumberType : public Type {
public:
    std::string unit;
    
    explicit NumberType(const std::string& u = "") : unit(u) {}
    
    bool isAssignableFrom(const Type& other) const override {
        if (auto* num = dynamic_cast<const NumberType*>(&other)) {
            return unit.empty() || num->unit == unit;
        }
        return false;
    }
    
    std::string toString() const override {
        return unit.empty() ? "number" : "number<" + unit + ">";
    }
};

class StringType : public Type {
public:
    bool isAssignableFrom(const Type& other) const override {
        return dynamic_cast<const StringType*>(&other) != nullptr;
    }
    
    std::string toString() const override {
        return "string";
    }
};

class BooleanType : public Type {
public:
    bool isAssignableFrom(const Type& other) const override {
        return dynamic_cast<const BooleanType*>(&other) != nullptr;
    }
    
    std::string toString() const override {
        return "boolean";
    }
};

class FunctionType : public Type {
public:
    std::vector<std::shared_ptr<Type>> parameterTypes;
    std::shared_ptr<Type> returnType;
    
    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> ret)
        : parameterTypes(std::move(params)), returnType(std::move(ret)) {}
    
    bool isAssignableFrom(const Type& other) const override {
        if (auto* func = dynamic_cast<const FunctionType*>(&other)) {
            if (parameterTypes.size() != func->parameterTypes.size()) return false;
            if (!returnType->isAssignableFrom(*func->returnType)) return false;
            
            for (size_t i = 0; i < parameterTypes.size(); ++i) {
                if (!func->parameterTypes[i]->isAssignableFrom(*parameterTypes[i])) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    
    std::string toString() const override {
        std::string result = "fn(";
        for (size_t i = 0; i < parameterTypes.size(); ++i) {
            if (i > 0) result += ", ";
            result += parameterTypes[i]->toString();
        }
        result += ") -> " + returnType->toString();
        return result;
    }
};

class UnitType : public Type {
public:
    std::string name;
    
    explicit UnitType(const std::string& n) : name(n) {}
    
    bool isAssignableFrom(const Type& other) const override {
        if (auto* unit = dynamic_cast<const UnitType*>(&other)) {
            return name == unit->name;
        }
        return false;
    }
    
    std::string toString() const override {
        return name;
    }
};

// Type registry for unit conversions
class TypeRegistry {
private:
    std::unordered_map<std::string, std::shared_ptr<Type>> types;
    std::unordered_map<std::string, std::unordered_map<std::string, double>> unitConversions;
    
public:
    void registerType(const std::string& name, std::shared_ptr<Type> type) {
        types[name] = std::move(type);
    }
    
    void registerUnitConversion(const std::string& from, const std::string& to, double factor) {
        unitConversions[from][to] = factor;
    }
    
    std::shared_ptr<Type> getType(const std::string& name) const {
        auto it = types.find(name);
        return it != types.end() ? it->second : nullptr;
    }
    
    double getConversionFactor(const std::string& from, const std::string& to) const {
        auto it = unitConversions.find(from);
        if (it != unitConversions.end()) {
            auto jt = it->second.find(to);
            if (jt != it->second.end()) {
                return jt->second;
            }
        }
        return 1.0;  // Default to no conversion
    }
};

} // namespace gehu 