#include "ast.h"

ExprType getExprType(Expr* e, ntr::env::Env &env) {
        if (auto* num = dynamic_cast<NumberExpr*>(e)) return ExprType::Number;
        if (auto* str = dynamic_cast<StrExpr*>(e)) return ExprType::String;
        if (auto* bin = dynamic_cast<BinaryExpr*>(e)) {
            ExprType l = getExprType(bin->left.get(), env);
            ExprType r = getExprType(bin->right.get(), env);
            if (l != r) throw std::runtime_error("Type error: mixed types");
            return l;
        }
        if (auto* var = dynamic_cast<VarExpr*>(e)) {
            Expr* value = env.getLet(var->name);
            if (!value) throw std::runtime_error("Undefined variable: " + var->name);
            return getExprType(value, env);
        }
        throw std::runtime_error("Unknown expression type");
}