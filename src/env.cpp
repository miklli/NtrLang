#include "env.h"

#pragma once
using namespace std;

namespace ntr::env {
    void Env::setLet(const std::string &name, const Value &v) {
        this->env.insert(pair<string, Value>(move(name), move(v)));
    }

    Value Env::getLet(const string &name) {
        auto it = this->env.find(name);
        if(it == this->env.end()) throw runtime_error("Unknow let");
        return it->second;
    }

    void Value::print() {
        std::visit([](auto&& arg) {
            std::cout << arg << std::endl;
        }, value);
    }
}