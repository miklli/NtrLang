#include "ast.h"

namespace ntr{
    struct Instruct {
        virtual ~Instruct() = default;
    };

    struct PUSH : Instruct {
        PUSH(int v);
        PUSH(std::string v);
        std::optional<int> value_int;
        std::optional<std::string> value_str;
    };

    struct STORE : Instruct {
        STORE(std::string name);
        std::string name;
    };

    struct LOAD : Instruct {
        LOAD(std::string name);
        std::string name;
    };

    struct PRINT : Instruct {};
    struct ADD : Instruct {};
    struct SUB : Instruct {};
    struct MUL : Instruct {};
    struct DIV : Instruct {};

    class Compiler {
    public:
        Compiler(std::vector<std::unique_ptr<Stmt>>& statem);
        void compile();
        void printI();
    private:
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