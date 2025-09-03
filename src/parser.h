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

        std::unique_ptr<Expr> parseNumber();
        std::unique_ptr<Expr> parseFactor();
        std::unique_ptr<Expr> parseTerm();
        std::unique_ptr<Expr> parseExpr();
        std::unique_ptr<Expr> parseStr();
        std::unique_ptr<Stmt> parseStmt();
        
        template <typename T>
        T expect(std::string msg);

        bool isIdent(const std::string &kw);
    public:
        std::vector<unique_ptr<Stmt>> statements; 
        Parser(const vector<Any> &tokens);
        void Parse();
    };
}