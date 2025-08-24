#include "interpreter.h"
#include "env.h"

namespace ntr::interpreter {
    Interpreter::Interpreter(ntr::env::Env &env): env(env) {}

    void Interpreter::run(const std::vector<unique_ptr<Stmt>> &functions) {
        for(const auto &stmt : functions) {
            stmt->exec(env);
        }
    }
};