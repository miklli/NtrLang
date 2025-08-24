#include "token.h"
#include "env.h"
#pragma once

enum class Operator {Plus, Minus, Multip, Slash};

struct Expr { 
    virtual ~Expr() = default;
    virtual int eval(ntr::env::Env &env) const = 0;
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int value): value(std::move(value)) {}
    int eval(ntr::env::Env &env) const override {
        return value;
    }
};

struct VarExpr : Expr {
    std::string name;
    VarExpr(std::string name): name(std::move(name)) {}
    int eval(ntr::env::Env &env) const override {
        ntr::env::Value let = env.getLet(this->name);
        if(let.type != ntr::env::ValueType::Int) {
            throw runtime_error("Invalid variable type");
        }
        return get<int>(let.value);
    }
};

struct BinaryExpr : Expr {
    unique_ptr<Expr> left, right;
    Operator op; 
    BinaryExpr(Operator o, unique_ptr<Expr> l, unique_ptr<Expr> r): op(o), left(std::move(l)), right(std::move(r))  {}
    int eval(ntr::env::Env &env) const override {
        if(op == Operator::Plus) return left->eval(env) + right->eval(env);
        if(op == Operator::Minus) return left->eval(env) - right->eval(env);
        if(op == Operator::Multip) return left->eval(env) * right->eval(env);
        if(op == Operator::Slash) return left->eval(env) / right->eval(env);
        throw runtime_error("Unknow operator");
    }
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void exec(ntr::env::Env &env) const = 0;
    virtual void printStmt() {}
};

struct LetStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;
    LetStmt(std::string name, std::unique_ptr<Expr> &value): 
        name(std::move(name)),
        value(std::move(value)) {}
    void exec(ntr::env::Env &env) const override  {
        env.setLet(name, ntr::env::Value(value->eval(env)));
    }
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> value;
    PrintStmt(std::unique_ptr<Expr> &val) 
        : value(std::move(val)) {}
    void exec(ntr::env::Env &env) const override {
        ntr::env::Value(value->eval(env)).print();
    }
};
