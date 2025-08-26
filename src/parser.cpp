#include "parser.h"

namespace ntr {
    Any Parser::peek() { return tokens[pos]; }
    Any Parser::get() { return tokens[pos++]; }

    Parser::Parser(const vector<Any> &tokens): tokens(move(tokens)) {}

    std::unique_ptr<Expr> Parser::parseNumber(ntr::env::Env &env) {
        if(std::holds_alternative<Integer>(peek())) {
            return make_unique<NumberExpr>(std::get<Integer>(get()).value);
        }
        throw runtime_error("Syntax error");
    }

    std::unique_ptr<Expr> Parser::parseStr(ntr::env::Env &env) {
        if(std::holds_alternative<StringLiteral>(peek())) {
            return std::make_unique<StrExpr>(std::get<StringLiteral>(get()).value);
        }
        throw runtime_error("Expected string literal");
    }

    std::unique_ptr<Expr> Parser::parseFactor(ntr::env::Env &env) {
        if(std::holds_alternative<Integer>(peek())) return parseNumber(env);
        if(std::holds_alternative<StringLiteral>(peek())) return parseStr(env);
        if(std::holds_alternative<Identifier>(peek())) 
            return std::make_unique<VarExpr>(std::get<Identifier>(get()).value);
        if(std::holds_alternative<LPar>(peek())) {
            get();
            auto expr = parseExpr(env);
            if(std::holds_alternative<RPar>(peek()))
                get();
            return expr; 
        }
        throw runtime_error("Unknow expr");
    }

    std::unique_ptr<Expr> Parser::parseTerm(ntr::env::Env &env) {
        auto left = parseFactor(env);
        while(std::holds_alternative<Multi>(peek())) {
            Operator op = Operator::Multip;
            get();
            auto right = parseFactor(env);
            left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), env);
        }
        return left;
    }

    std::unique_ptr<Expr> Parser::parseExpr(ntr::env::Env &env) {
        auto left = parseTerm(env);
        if(std::holds_alternative<endTok>(peek())) return left;
        while(std::holds_alternative<Plus>(peek())) {
            Operator op = Operator::Plus;
            get();
            auto right = parseTerm(env);
            left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), env);
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
            auto expr = parseExpr(env);
            expect<Semicolon>("expected ';'.");
            return std::make_unique<LetStmt>(name, std::move(expr), env);
        }
        else if(isIdent("print")) {
            get(); //print
            expect<LPar>("expected '('.");
            auto expr = parseExpr(env);
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