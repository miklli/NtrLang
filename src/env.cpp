#include "env.h"
#include "ast.h"

using namespace std;

namespace ntr::env {
    void Env::setLet(const std::string &name, std::unique_ptr<Expr> v) {
        this->env.insert(pair<string, std::unique_ptr<Expr>>(move(name), move(v)));
    }

    Expr* Env::getLet(const std::string& name) {
        auto it = env.find(name);
        if(it == env.end() || !it->second)
            throw std::runtime_error("Unknown variable: " + name);
        return it->second.get();
    }

    bool Env::has(const std::string &name) const {
        return env.find(name) != env.end();
    }
}