#include "ast.hpp"
#include "../semantic/type.hpp"

namespace gehu {

TypePtr BinaryExpr::getType() const {
    // Default to number type for now
    // This will be properly implemented in the semantic analyzer
    return std::make_shared<NumberType>();
}

TypePtr UnaryExpr::getType() const {
    // Default to number type for now
    // This will be properly implemented in the semantic analyzer
    return std::make_shared<NumberType>();
}

TypePtr LiteralExpr::getType() const {
    if (std::holds_alternative<double>(value)) {
        return std::make_shared<NumberType>(unit);
    } else {
        return std::make_shared<StringType>();
    }
}

TypePtr VarExpr::getType() const {
    // This will be resolved by the semantic analyzer
    return nullptr;
}

TypePtr AssignExpr::getType() const {
    // Return the type of the assigned value
    return value->getType();
}

TypePtr CallExpr::getType() const {
    // This will be resolved by the semantic analyzer
    return nullptr;
}

} // namespace gehu 