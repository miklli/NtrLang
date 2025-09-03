#include "token.h"
#pragma once

enum class Operator {Plus, Minus, Multi, Slash};

enum class ExprType { Number, String };

struct Expr { 
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int value): value(std::move(value)) {}
};

struct StrExpr : Expr {
    std::string value;
    StrExpr(std::string v): value(std::move(v)) {}
};

struct BinaryExpr : Expr {
    unique_ptr<Expr> left, right;
    Operator op; 
    ExprType exprType;
    BinaryExpr(Operator o, unique_ptr<Expr> l, unique_ptr<Expr> r): op(o), left(std::move(l)), right(std::move(r))  {}
};

struct VarExpr : Expr {
    std::string name;
    VarExpr(std::string name): name(std::move(name)) {}
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;
    LetStmt(std::string name, std::unique_ptr<Expr> value): 
        name(std::move(name)),
        value(std::move(value)) {}
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> value;
    PrintStmt(std::unique_ptr<Expr> val) 
        : value(std::move(val)) {}
};
