#include "token.h"
#include "env.h"
#pragma once

enum class Operator {Plus, Minus, Multi, Slash};

enum class ExprType { Number, String };
ExprType getExprType(Expr* e, ntr::env::Env &env); 

struct Expr { 
    virtual ~Expr() = default;
    virtual int eval(ntr::env::Env &env) {
        throw std::runtime_error("This expression cannot be evaluated as int");
    }
    virtual std::string evalStr(ntr::env::Env &env) {
        throw std::runtime_error("This expression cannot be evaluated as string");
    }
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int value): value(std::move(value)) {}
    int eval(ntr::env::Env &env) override {
        return value;
    }
};

struct StrExpr : Expr {
    std::string value;
    StrExpr(std::string v): value(std::move(v)) {}
    std::string evalStr(ntr::env::Env &env) {
        return value;
    }
};


struct BinaryExpr : Expr {
    unique_ptr<Expr> left, right;
    Operator op; 
    ExprType exprType;
    BinaryExpr(Operator o, unique_ptr<Expr> l, unique_ptr<Expr> r, ntr::env::Env &env): op(o), left(std::move(l)), right(std::move(r))  {}
    int eval(ntr::env::Env &env) override {
        if(op == Operator::Plus) return left->eval(env) + right->eval(env);
        if(op == Operator::Minus) return left->eval(env) - right->eval(env);
        if(op == Operator::Multi) return left->eval(env) * right->eval(env);
        if(op == Operator::Slash) return left->eval(env) / right->eval(env);
        throw runtime_error("Unknow operator");
    }
    std::string evalStr(ntr::env::Env &env) override {
        if(op == Operator::Plus) return left->evalStr(env) + right->evalStr(env);
        throw runtime_error("String has only one operator '+'");
    }
    
    void chek(ntr::env::Env &env) {
        ExprType leftType = getExprType(left.get(), env);
        ExprType rightType = getExprType(right.get(), env);

        if (op == Operator::Plus) {
            if (leftType != rightType) {
                throw std::runtime_error("Type error: cannot mix string and int with '+'");
            }
            this->exprType = leftType;
        } else { 
            if (leftType != ExprType::Number || rightType != ExprType::Number) {
                throw std::runtime_error("Type error: arithmetic operators require numbers");
            }
            this->exprType = ExprType::Number;
        }
    }
};

struct VarExpr : Expr {
    std::string name;
    VarExpr(std::string name): name(std::move(name)) {}

    int eval(ntr::env::Env &env) override {
        Expr* let = env.getLet(this->name);
        if (auto* num = dynamic_cast<NumberExpr*>(let))
            return num->eval(env);
        if (auto* bin = dynamic_cast<BinaryExpr*>(let))
            return bin->eval(env);
        if (auto* var = dynamic_cast<VarExpr*>(let))
            return var->eval(env); 
        throw std::runtime_error("Variable is not an int");
    }

    std::string evalStr(ntr::env::Env &env) override {
        Expr* let = env.getLet(this->name);
        if (auto* str = dynamic_cast<StrExpr*>(let))
            return str->evalStr(env);
        if (auto* bs = dynamic_cast<BinaryExpr*>(let))
            return bs->evalStr(env);
        if (auto* var = dynamic_cast<VarExpr*>(let))
            return var->evalStr(env); 
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
        value(std::move(value)) {
            if (containsVar(this->value.get(), this->name)) {
                throw std::runtime_error("Recursive assignment to variable '" + this->name + "'");
            }
        }

    void exec(ntr::env::Env &env) override  {
        if (auto* bin = dynamic_cast<BinaryExpr*>(value.get())) {
            bin->chek(env); 
        } else {
            getExprType(value.get(), env);
        }
        env.setLet(name, std::move(value));
    }
private:
    bool containsVar(Expr* expr, const std::string& varName) {
        if (!expr) return false;
        if (auto* var = dynamic_cast<VarExpr*>(expr)) {
            return var->name == varName;
        }
        if (auto* bin = dynamic_cast<BinaryExpr*>(expr)) {
            return containsVar(bin->left.get(), varName) || containsVar(bin->right.get(), varName);
        }
        return false;
    }
};

struct PrintStmt : Stmt {
    std::unique_ptr<Expr> value;
    PrintStmt(std::unique_ptr<Expr> val) 
        : value(std::move(val)) {}
    void exec(ntr::env::Env &env) override {
        if (auto* bin = dynamic_cast<BinaryExpr*>(value.get())) {
            bin->chek(env); 
        } else {
            getExprType(value.get(), env);
        }
        try {
            std::cout << value->eval(env) << '\n';
            return;
        } catch (const std::runtime_error&) {
        }

        try {
            std::cout << value->evalStr(env) << '\n';
            return;
        } catch (const std::runtime_error&) {
        }

        throw std::runtime_error("Unsupported expression type in print");
    }
};
