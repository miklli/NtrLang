#include <bits/stdc++.h>
#include "token.h"
#include "lexer.h"

using namespace std;

namespace ntr::lexer {
    vector<ntr::token::Any> process(const string& code) {
        vector<ntr::token::Any> result;
        for (auto it = code.begin(); it != code.end(); it++) {
            switch (*it) {
                default:
                    if(isalpha(*it)) {
                        auto blank = std::find_if(it, code.end(), [](char c){
                            return c == ' ' || c == '\n' || c == '\t' || c == '\r' || (!isalpha(c) && !isdigit(c));
                        });
                        string StringValue(it, blank);
                        static map<string, ntr::token::Keyword> keywords = {
                            {"return", ntr::token::Keyword::RETURN},
                            {"fn", ntr::token::Keyword::FN}
                        };
                        if(auto keyword = keywords.find(StringValue); keyword != keywords.end()) {
                            result.emplace_back(keyword->second);
                            it = prev(blank);
                            break;
                        }
                        result.emplace_back(ntr::token::Identifier{ .value=StringValue });
                        it = prev(blank);
                        break;
                    }
                    
                    if(isdigit(*it)) {
                        auto blank = std::find_if(it, code.end(), [](char c){
                            return !isdigit(c);
                        });
                        result.emplace_back(ntr::token::Integer{ .value=stoi(string(it, blank)) });
                        it = prev(blank);
                        break;
                    }

                    throw runtime_error("Unknow operator");
                    break;
                case ' ':
                    break;
                case '\r':
                    break;
                case '\n':
                    break;
                case '\t':
                    break;
                case '(':
                    result.emplace_back(ntr::token::LPar{});
                    break;
                case '#':
                    while(*it != '\n' && it != std::prev(code.end())) it++;
                    break;
                case ')':
                    result.emplace_back(ntr::token::RPar{});
                    break;
                case '{':
                    result.emplace_back(ntr::token::LCB{});
                    break;
                case '}':
                    result.emplace_back(ntr::token::RCB{});
                    break;
                case ';':
                    result.emplace_back(ntr::token::Semicolon{});
                    break;
                case '+':
                    result.emplace_back(ntr::token::Plus{});
                    break;
                case '=':
                    result.emplace_back(ntr::token::Equals{});
                    break;
                case '*':
                    result.emplace_back(ntr::token::Multi{});
                    break;
                case '-':
                    it++;
                    while(*it == ' ' || *it == '\t') {
                        it++;
                    }
                    if(isdigit(*it)) {
                        auto blank = std::find_if(it, code.end(), [](char c){
                            return !isdigit(c);
                        });
                        if(std::holds_alternative<ntr::token::Integer>(result.back())) {
                            result.emplace_back(ntr::token::Plus{});
                        }
                        result.emplace_back(ntr::token::Integer{ .value=-stoi(string(it, blank)) });
                        it = prev(blank);
                        break;
                    }
                    result.emplace_back(ntr::token::Minus{});
                    break;
                case '/':
                    result.emplace_back(ntr::token::Slash{});
                    break;
                case '"':
                    std::string str;
                    it++;
                    while(*it != '"') {
                        str += *it;
                        it++;
                    }
                    result.emplace_back(ntr::token::StringLiteral{ .value=str });
                    break;
            }
        }
        result.emplace_back(ntr::token::endTok{});
        return result;
    }
}