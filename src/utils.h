#include "ast.h"
#include "compiler.h"

#pragma once

void IntToBin(std::vector<uint8_t>& out, int32_t value);

void StrToBin(std::vector<uint8_t>& out, const std::string& s);

ExprType getExprType(Expr* e, ntr::SemanticTable STab);

bool isStmtValid(Stmt* stmt, ntr::SemanticTable &STab);