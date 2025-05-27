#pragma once

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace gehu {

class Type {
public:
    virtual ~Type() = default;
    virtual bool isAssignableFrom(const Type& other) const = 0;
    virtual std::string toString() const = 0;
};

class NumberType : public Type {
public:
    bool isAssignableFrom(const Type& other) const override;
    std::string toString() const override { return "number"; }
};

class StringType : public Type {
public:
    bool isAssignableFrom(const Type& other) const override;
    std::string toString() const override { return "string"; }
};

class UnitType : public Type {
public:
    explicit UnitType(const std::string& unit) : unit_(unit) {}
    bool isAssignableFrom(const Type& other) const override;
    std::string toString() const override { return unit_; }
    
    const std::string& unit() const { return unit_; }

private:
    std::string unit_;
};

class FunctionType : public Type {
public:
    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> return_type)
        : params_(std::move(params)), return_type_(std::move(return_type)) {}
    
    bool isAssignableFrom(const Type& other) const override;
    std::string toString() const override;

    const std::vector<std::shared_ptr<Type>>& params() const { return params_; }
    const std::shared_ptr<Type>& returnType() const { return return_type_; }

private:
    std::vector<std::shared_ptr<Type>> params_;
    std::shared_ptr<Type> return_type_;
};

} // namespace gehu 