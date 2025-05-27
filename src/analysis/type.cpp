#include "type.hpp"
#include <sstream>

namespace gehu {

bool NumberType::isAssignableFrom(const Type& other) const {
    return dynamic_cast<const NumberType*>(&other) != nullptr;
}

bool StringType::isAssignableFrom(const Type& other) const {
    return dynamic_cast<const StringType*>(&other) != nullptr;
}

bool UnitType::isAssignableFrom(const Type& other) const {
    if (const auto* other_unit = dynamic_cast<const UnitType*>(&other)) {
        return unit_ == other_unit->unit_;
    }
    return false;
}

bool FunctionType::isAssignableFrom(const Type& other) const {
    if (const auto* other_func = dynamic_cast<const FunctionType*>(&other)) {
        if (params_.size() != other_func->params_.size()) return false;
        if (!return_type_->isAssignableFrom(*other_func->return_type_)) return false;
        
        for (size_t i = 0; i < params_.size(); ++i) {
            if (!other_func->params_[i]->isAssignableFrom(*params_[i])) return false;
        }
        return true;
    }
    return false;
}

std::string FunctionType::toString() const {
    std::stringstream ss;
    ss << "func(";
    for (size_t i = 0; i < params_.size(); ++i) {
        ss << params_[i]->toString();
        if (i < params_.size() - 1) ss << ", ";
    }
    ss << "): " << return_type_->toString();
    return ss.str();
}

} // namespace gehu 