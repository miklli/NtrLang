#include <bits/stdc++.h>

#pragma once

struct Expr;

namespace ntr::env {

    class Env {
    public:
        void setLet(const std::string &name, std::unique_ptr<Expr> v);
        Expr* getLet(const std::string &name);
        bool has(const std::string &name) const;
    private:
        std::unordered_map<std::string, std::unique_ptr<Expr>> env;
    };
}