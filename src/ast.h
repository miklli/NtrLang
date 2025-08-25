#include "token.h"
#include "env.h"
#pragma once

enum class Operator {Plus, Minus, Multip, Slash};

struct Expr { 
    virtual ~Expr() = default;
    virtual int eval(ntr::env::Env &env) const {
        throw std::runtime_error("This expression cannot be evaluated as int");
    }
    virtual std::string evalStr(ntr::env::Env &env) const {
        throw std::runtime_error("This expression cannot be evaluated as string");
    }
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int value): value(std::move(value)) {}
    int eval(ntr::env::Env &env) const override {
        return value;
    }
};

struct StrExpr : Expr {
    std::string value;
    StrExpr(std::string v): value(std::move(v)) {}
    std::string evalStr(ntr::env::Env &env) const {
        return value;
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

struct VarExpr : Expr {
    std::string name;
    VarExpr(std::string name): name(std::move(name)) {}

    int eval(ntr::env::Env &env) const override {
        Expr* let = env.getLet(this->name);
        if (auto num = dynamic_cast<BinaryExpr*>(let)) return num->eval(env);
        else if (auto num = dynamic_cast<NumberExpr*>(let)) return num->eval(env);
        throw std::runtime_error("Variable is not an int");
    }

    std::string evalString(ntr::env::Env &env) const {
        Expr* let = env.getLet(this->name);
        if (auto str = dynamic_cast<StrExpr*>(let)) return str->evalStr(env);
        throw std::runtime_error("Variable is not a string");
    }
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual void exec(ntr::env::Env &env) = 0;
    virtual void printStmt() {}
};

struct LetStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;
    LetStmt(std::string name, std::unique_ptr<Expr> value): 
        name(std::move(name)),
        value(std::move(value)) {}
    void exec(ntr::env::Env &env) override  {
        if (auto num = dynamic_cast<NumberExpr*>(value.get()))
            value->eval(env);
        env.setLet(name, std::move(value));
    }
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> value;
    PrintStmt(std::unique_ptr<Expr> val) 
        : value(std::move(val)) {}
    void exec(ntr::env::Env &env) override {
        const Expr* e = value.get();

        if (auto* num = dynamic_cast<const NumberExpr*>(e)) {
            std::cout << num->eval(env);
            std::cout << '\n';
            return;
        }
        if (auto* str = dynamic_cast<const StrExpr*>(e)) {
            std::cout << str->evalStr(env);
            std::cout << '\n';
            return;
        }
        if (auto* var = dynamic_cast<const VarExpr*>(e)) {
            Expr* let = env.getLet(var->name);

            if (auto* n = dynamic_cast<NumberExpr*>(let)) {
                std::cout << n->eval(env);
                std::cout << '\n';
                return;
            }
            if (auto* s = dynamic_cast<StrExpr*>(let)) {
                std::cout << s->evalStr(env);
                std::cout << '\n';
                return;
            }
            if (auto* b = dynamic_cast<BinaryExpr*>(let)) {
                std::cout << b->eval(env);
                std::cout << '\n';
                return;
            }
            throw std::runtime_error("Unsupported variable type in print");
        }
        if (auto* bin = dynamic_cast<const BinaryExpr*>(e)) {
            std::cout << bin->eval(env); 
            std::cout << '\n';
            return;
        }

        throw std::runtime_error("Unsupported expression type in print");
    }
};
