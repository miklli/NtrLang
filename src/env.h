#include <bits/stdc++.h>

#pragma once

namespace ntr::env {

    enum class ValueType {Int, Str, Bool};

    struct Value {
        ValueType type;
        std::variant<int, std::string, bool> value;
        Value(int value): type(ValueType::Int), value(value) {}
        void print();
    };

    class Env {
    public:
        void setLet(const std::string &name, const Value &v);
        Value getLet(const std::string &name);
    private:
        std::unordered_map<std::string, Value> env;
    };
}