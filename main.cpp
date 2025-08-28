#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "compiler.h"

using namespace ntr::token;
using namespace std;

void print(const Any &token) {
    if(auto value = get_if<ntr::token::Identifier>(&token)) {
        cout << "Identifier " << value->value  << '\n';
    }
    if(auto value = get_if<ntr::token::StringLiteral>(&token)) {
        cout << "StringLiteral " << value->value  << '\n';
    }
    else if(auto value = get_if<ntr::token::LPar>(&token)) {
        cout << "LPar" << '\n';
    }
    else if(auto value = get_if<ntr::token::RPar>(&token)) {
        cout << "RPar" << '\n';
    }
    else if(auto value = get_if<ntr::token::Integer>(&token)) {
        cout << "Integer " << value->value << '\n';
    }
    else if(auto value = get_if<ntr::token::Plus>(&token)) {
        cout << "Plus" << '\n';
    }
    else if(auto value = get_if<ntr::token::LCB>(&token)) {
        cout << "LCB" << '\n';
    }
    else if(auto value = get_if<ntr::token::RCB>(&token)) {
        cout << "RCB" << '\n';
    }
    else if(auto value = get_if<ntr::token::Equals>(&token)) {
        cout << "Equals" << '\n';
    }
    else if(auto value = get_if<ntr::token::Semicolon>(&token)) {
        cout << "Semicolon" << '\n';
    }
    else if(auto value = get_if<ntr::token::Minus>(&token)) {
        cout << "Minus" << '\n';
    }
    else if(auto value = get_if<ntr::token::Multi>(&token)) {
        cout << "Multi" << '\n';
    }
    else if(auto value = get_if<ntr::token::Slash>(&token)) {
        cout << "Slash" << '\n';
    }
    else if(auto value = get_if<ntr::token::Keyword>(&token)) {
        cout << "Keyword ";
        switch (*value) {
            case ntr::token::Keyword::FN:
                cout << "FN";
                break;
            case ntr::token::Keyword::RETURN:
                cout << "RETRUN";
                break;
        }
        cout << '\n';
    }
}

int main(int argc, char* argv[]) {
    if(argc > 2) {
        cout << "Only 1 argument is passed to the input!";
        return 1;
    }
    else if (argc  == 1) {
        cout << "Run 'ntr {ntr code file}'";
        return 0;
    }
    
    string codeFile = argv[1];

    try {
        std::ifstream file(codeFile);
        if (!file) {
            std::cerr << "Cannot open file\n";
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string code = buffer.str();
        auto processed = ntr::lexer::process(code);
        ntr::env::Env env;
        ntr::Parser parser(processed);
        ntr::interpreter::Interpreter inter(env);
        parser.Parse(env);
        ntr::Compiler compiler(parser.statements);
        //inter.run(parser.statements);
        compiler.compile();
        compiler.printI();
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << '\n';
    }
    return 0;
}