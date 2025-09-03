#include "utils.h"
#include "compiler.h"

void IntToBin(std::vector<uint8_t>& out, int32_t value) {
    for (int i = 0; i < 4; i++)
        out.push_back((value >> (8 * i)) & 0xFF);
}

void StrToBin(std::vector<uint8_t>& out, const std::string& s) {
    out.push_back((uint8_t)s.size());
    out.insert(out.end(), s.begin(), s.end());
}

ExprType getExprType(Expr* e, ntr::SemanticTable STab) {
        if (auto* num = dynamic_cast<NumberExpr*>(e)) return ExprType::Number;
        if (auto* str = dynamic_cast<StrExpr*>(e)) return ExprType::String;
        if (auto* bin = dynamic_cast<BinaryExpr*>(e)) {
            ExprType l = getExprType(bin->left.get(), STab);
            ExprType r = getExprType(bin->right.get(), STab);
            if (l != r) throw std::runtime_error("The expression is not correct.");
            return l;
        }
        if (auto* var = dynamic_cast<VarExpr*>(e)) {
            return STab.getVarType(var->name);
        }
        throw std::runtime_error("Unknown expression type.");
}

bool isStmtValid(Stmt* stmt, ntr::SemanticTable &STab) {
    if(auto* Setlet = dynamic_cast<LetStmt*>(stmt)) {
        getExprType(Setlet->value.get(), STab);
        STab.setVarType(Setlet->name, Setlet->value.get());
        return true;
    }
    else if(auto* print = dynamic_cast<PrintStmt*>(stmt)) {
        getExprType(print->value.get(), STab);
        return true;
    }
    return false;
}