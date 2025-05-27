#pragma once

#include "symbol.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace gehu {

class Scope {
public:
    Scope() = default;
    explicit Scope(std::shared_ptr<Scope> parent) : parent_(std::move(parent)) {}

    void define(std::shared_ptr<Symbol> symbol) {
        symbols_[symbol->name()] = std::move(symbol);
    }

    std::shared_ptr<Symbol> resolve(const std::string& name) const {
        auto it = symbols_.find(name);
        if (it != symbols_.end()) {
            return it->second;
        }
        if (parent_) {
            return parent_->resolve(name);
        }
        return nullptr;
    }

    const std::shared_ptr<Scope>& parent() const { return parent_; }

private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols_;
    std::shared_ptr<Scope> parent_;
};

} // namespace gehu 