#include <bits/stdc++.h>
#include "ast.h"

namespace ntr::interpreter {
    class Interpreter {
    private:
        ntr::env::Env &env;
    public:
        Interpreter(ntr::env::Env &env);
        void run(const std::vector<unique_ptr<Stmt>> &functions);
    };
}