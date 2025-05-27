#pragma once
#include <string>
#include <vector>
#include <memory>
#include <variant>

namespace gehu {

// Forward declarations for all AST node classes
class Expr;
class Stmt;
class Type;
class BinaryExpr;
class UnaryExpr;
class LiteralExpr;
class VarExpr;
class AssignExpr;
class CallExpr;
class ExprStmt;
class LetStmt;
class ShowStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class ForStmt;
class MatchStmt;
class SpawnStmt;
class WaitStmt;
class ReturnStmt;

using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;
using TypePtr = std::shared_ptr<Type>;

// Visitor interfaces
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual TypePtr visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual TypePtr visitUnaryExpr(UnaryExpr& expr) = 0;
    virtual TypePtr visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual TypePtr visitVarExpr(VarExpr& expr) = 0;
    virtual TypePtr visitAssignExpr(AssignExpr& expr) = 0;
    virtual TypePtr visitCallExpr(CallExpr& expr) = 0;
};

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual void visitExprStmt(ExprStmt& stmt) = 0;
    virtual void visitLetStmt(LetStmt& stmt) = 0;
    virtual void visitShowStmt(ShowStmt& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
    virtual void visitForStmt(ForStmt& stmt) = 0;
    virtual void visitMatchStmt(MatchStmt& stmt) = 0;
    virtual void visitSpawnStmt(SpawnStmt& stmt) = 0;
    virtual void visitWaitStmt(WaitStmt& stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt& stmt) = 0;
};

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual TypePtr getType() const = 0;
    virtual TypePtr accept(ExprVisitor& visitor) = 0;
};

// Base statement class
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) = 0;
};

// Expression types
class BinaryExpr : public Expr {
public:
    enum class Op {
        Add, Sub, Mul, Div,
        Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual,
        Pipeline
    };
    
    ExprPtr left;
    Op op;
    ExprPtr right;
    
    BinaryExpr(ExprPtr l, Op o, ExprPtr r) : left(l), op(o), right(r) {}
    TypePtr getType() const override;
    TypePtr accept(ExprVisitor& visitor) override { return visitor.visitBinaryExpr(*this); }
};

class UnaryExpr : public Expr {
public:
    enum class Op { Negate, Not };
    
    Op op;
    ExprPtr right;
    
    UnaryExpr(Op o, ExprPtr r) : op(o), right(r) {}
    TypePtr getType() const override;
    TypePtr accept(ExprVisitor& visitor) override { return visitor.visitUnaryExpr(*this); }
};

class LiteralExpr : public Expr {
public:
    std::variant<double, std::string> value;
    std::string unit;  // For unit literals
    
    LiteralExpr(const std::variant<double, std::string>& v, const std::string& u = "")
        : value(v), unit(u) {}
    TypePtr getType() const override;
    TypePtr accept(ExprVisitor& visitor) override { return visitor.visitLiteralExpr(*this); }
};

class VarExpr : public Expr {
public:
    std::string name;
    
    explicit VarExpr(const std::string& n) : name(n) {}
    TypePtr getType() const override;
    TypePtr accept(ExprVisitor& visitor) override { return visitor.visitVarExpr(*this); }
};

class AssignExpr : public Expr {
public:
    std::string name;
    ExprPtr value;
    
    AssignExpr(const std::string& n, ExprPtr v) : name(n), value(v) {}
    TypePtr getType() const override;
    TypePtr accept(ExprVisitor& visitor) override { return visitor.visitAssignExpr(*this); }
};

class CallExpr : public Expr {
public:
    ExprPtr callee;
    std::vector<ExprPtr> arguments;
    
    CallExpr(ExprPtr c, std::vector<ExprPtr> args) : callee(c), arguments(std::move(args)) {}
    TypePtr getType() const override;
    TypePtr accept(ExprVisitor& visitor) override { return visitor.visitCallExpr(*this); }
};

// Statement types
class ExprStmt : public Stmt {
public:
    ExprPtr expression;
    
    explicit ExprStmt(ExprPtr e) : expression(e) {}
    void accept(StmtVisitor& visitor) override { visitor.visitExprStmt(*this); }
};

class LetStmt : public Stmt {
public:
    std::string name;
    TypePtr type;
    ExprPtr initializer;
    
    LetStmt(const std::string& n, TypePtr t, ExprPtr i)
        : name(n), type(t), initializer(i) {}
    void accept(StmtVisitor& visitor) override { visitor.visitLetStmt(*this); }
};

class ShowStmt : public Stmt {
public:
    ExprPtr expression;
    
    explicit ShowStmt(ExprPtr e) : expression(e) {}
    void accept(StmtVisitor& visitor) override { visitor.visitShowStmt(*this); }
};

class BlockStmt : public Stmt {
public:
    std::vector<StmtPtr> statements;
    
    explicit BlockStmt(std::vector<StmtPtr> stmts) : statements(std::move(stmts)) {}
    void accept(StmtVisitor& visitor) override { visitor.visitBlockStmt(*this); }
};

class IfStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;
    
    IfStmt(ExprPtr c, StmtPtr t, StmtPtr e)
        : condition(c), thenBranch(t), elseBranch(e) {}
    void accept(StmtVisitor& visitor) override { visitor.visitIfStmt(*this); }
};

class WhileStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr body;
    
    WhileStmt(ExprPtr c, StmtPtr b) : condition(c), body(b) {}
    void accept(StmtVisitor& visitor) override { visitor.visitWhileStmt(*this); }
};

class ForStmt : public Stmt {
public:
    std::string variable;
    ExprPtr iterator;
    StmtPtr body;
    
    ForStmt(const std::string& v, ExprPtr i, StmtPtr b)
        : variable(v), iterator(i), body(b) {}
    void accept(StmtVisitor& visitor) override { visitor.visitForStmt(*this); }
};

class MatchStmt : public Stmt {
public:
    ExprPtr value;
    std::vector<std::pair<ExprPtr, StmtPtr>> cases;
    StmtPtr defaultCase;
    
    MatchStmt(ExprPtr v, std::vector<std::pair<ExprPtr, StmtPtr>> c, StmtPtr d)
        : value(v), cases(std::move(c)), defaultCase(d) {}
    void accept(StmtVisitor& visitor) override { visitor.visitMatchStmt(*this); }
};

class SpawnStmt : public Stmt {
public:
    StmtPtr statement;
    
    explicit SpawnStmt(StmtPtr s) : statement(s) {}
    void accept(StmtVisitor& visitor) override { visitor.visitSpawnStmt(*this); }
};

class WaitStmt : public Stmt {
public:
    ExprPtr expression;
    
    explicit WaitStmt(ExprPtr e) : expression(e) {}
    void accept(StmtVisitor& visitor) override { visitor.visitWaitStmt(*this); }
};

class ReturnStmt : public Stmt {
public:
    ExprPtr value;
    
    explicit ReturnStmt(ExprPtr v) : value(v) {}
    void accept(StmtVisitor& visitor) override { visitor.visitReturnStmt(*this); }
};

// Program
class Program {
public:
    std::vector<StmtPtr> statements;
    
    explicit Program(std::vector<StmtPtr> stmts) : statements(std::move(stmts)) {}
};

} // namespace gehu 