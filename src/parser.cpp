#include "parser.h"

namespace ntr {
    Any Parser::peek() { return tokens[pos]; }
    Any Parser::get() { return tokens[pos++]; }

    Parser::Parser(const vector<Any> &tokens): tokens(move(tokens)) {}

    std::unique_ptr<Expr> Parser::parseNumber() {
        if(std::holds_alternative<Integer>(peek())) {
            return make_unique<NumberExpr>(std::get<Integer>(get()).value);
        }
        throw runtime_error("Syntax error");
    }

    std::unique_ptr<Expr> Parser::parseFactor() {
        if(std::holds_alternative<Integer>(peek())) return parseNumber();
        if(std::holds_alternative<Identifier>(peek())) 
            return std::make_unique<VarExpr>(std::get<Identifier>(get()).value);
        if(std::holds_alternative<LPar>(peek())) {
            get();
            auto expr = parseExpr();
            if(std::holds_alternative<RPar>(peek()))
                get();
            return expr; 
        }
        throw runtime_error("Unknow expr");
    }

    std::unique_ptr<Expr> Parser::parseTerm() {
        auto left = parseFactor();
        while(std::holds_alternative<Multi>(peek())) {
            Operator op = Operator::Multip;
            get();
            auto right = parseFactor();
            left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expr> Parser::parseExpr() {
        auto left = parseTerm();
        if(std::holds_alternative<endTok>(peek())) return left;
        while(std::holds_alternative<Plus>(peek())) {
            Operator op = Operator::Plus;
            get();
            auto right = parseTerm();
            left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
        }
        return left;
    }

    template <typename T>
    T Parser::expect(std::string msg) {
        if (!std::holds_alternative<T>(peek()))
            throw runtime_error("Syntax error: " + msg);
        return std::get<T>(get());
    }

    bool Parser::isIdent(const std::string &kw) {
        return std::holds_alternative<Identifier>(peek()) &&
            std::get<Identifier>(peek()).value == kw;
    }

    std::unique_ptr<Stmt> Parser::parseStmt(ntr::env::Env &env) {
        if(isIdent("let")) {
            get(); //let
            std::string name = expect<Identifier>("expected let name.").value;
            expect<Equals>("expected '='.");
            auto expr = parseExpr();
            expect<Semicolon>("expected ';'.");
            return std::make_unique<LetStmt>(name, std::move(expr));
        }
        else if(isIdent("print")) {
            get(); //print
            expect<LPar>("expected '('.");
            auto expr = parseExpr();
            expect<RPar>("expected ')'.");
            expect<Semicolon>("expected ';'.");
            return std::make_unique<PrintStmt>(std::move(expr));
        }
        throw runtime_error("Syntax error");
    }

    void Parser::Parse(ntr::env::Env &env) {
        while(!std::holds_alternative<endTok>(peek())) {
            functions.push_back(std::move(parseStmt(env)));
        }
    }
}