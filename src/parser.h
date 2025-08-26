#include "token.h"
#include "ast.h"

using namespace ntr::token;

namespace ntr {
    class Parser {
    private:
        vector<Any> tokens;
        size_t pos = 0;
        Any peek();
        Any get();

        std::unique_ptr<Expr> parseNumber(ntr::env::Env &env);
        std::unique_ptr<Expr> parseFactor(ntr::env::Env &env);
        std::unique_ptr<Expr> parseTerm(ntr::env::Env &env);
        std::unique_ptr<Expr> parseExpr(ntr::env::Env &env);
        std::unique_ptr<Expr> parseStr(ntr::env::Env &env);
        std::unique_ptr<Stmt> parseStmt(ntr::env::Env &env);
        
        template <typename T>
        T expect(std::string msg);

        bool isIdent(const std::string &kw);
    public:
        std::vector<unique_ptr<Stmt>> functions; 
        Parser(const vector<Any> &tokens);
        void Parse(ntr::env::Env &env);
    };
}