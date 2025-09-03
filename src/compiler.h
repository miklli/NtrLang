#include "ast.h"

#pragma once

namespace ntr{

    struct SemanticTable {
        ExprType getVarType(std::string &name);
        void setVarType(std::string &name, Expr* value);
    private:
        std::unordered_map<std::string, ExprType> STab;
    };

    struct Instruct {
        virtual ~Instruct() = default;
        virtual uint8_t getOpCode() = 0;
    };

    struct PUSH : Instruct {
        PUSH(int v);
        PUSH(std::string v);
        std::variant<int, string> value;

        uint8_t getOpCode() {
            return 0x01;
        }
    };

    struct STORE : Instruct {
        STORE(std::string name);
        std::string name;

        uint8_t getOpCode() {
            return 0x02;
        }
    };

    struct LOAD : Instruct {
        LOAD(std::string name);
        std::string name;

        uint8_t getOpCode() {
            return 0x03;
        }
    };

    struct PRINT : Instruct {
        uint8_t getOpCode() {
            return 0x04;
        }
    };
    struct ADD : Instruct {
        uint8_t getOpCode() {
            return 0x05;
        }
    };
    struct SUB : Instruct {
        uint8_t getOpCode() {
            return 0x06;
        }
    };
    struct MUL : Instruct {
        uint8_t getOpCode() {
            return 0x07;
        }
    };
    struct DIV : Instruct {
        uint8_t getOpCode() {
            return 0x08;
        }
    };

    class Compiler {
    public:
        Compiler(std::vector<std::unique_ptr<Stmt>>& statem);
        void compile(ntr::SemanticTable &STab);
        void ToBinFile();
    private:
        void SemanticAnalysis(ntr::SemanticTable &STab);
        std::vector<std::unique_ptr<Instruct>> instucts;
        std::vector<std::unique_ptr<Stmt>>& statements;
        void SetLetStmt(std::unique_ptr<LetStmt> state);
        void cPrintStmt(std::unique_ptr<PrintStmt> state);
        void push(std::unique_ptr<Expr> expr);
        void store(std::string name);
        void print();
        void add();
        void sub();
        void mul();
        void div();
    };
}