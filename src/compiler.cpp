#include "compiler.h"

namespace ntr {
    PUSH::PUSH(int v): value_int(v) {}
    PUSH::PUSH(std::string v): value_str(v) {}

    STORE::STORE(std::string name): name(name) {}
    LOAD::LOAD(std::string name): name(name) {}

    Compiler::Compiler(std::vector<std::unique_ptr<Stmt>>& statem): statements(statem) {}
    
    void Compiler::compile() {
        for(const auto& state : statements) {
            if(auto l = dynamic_cast<LetStmt*>(state.get())) {
                SetLetStmt(std::make_unique<LetStmt>(l->name, std::move(l->value)));
            }
            else if(auto p = dynamic_cast<PrintStmt*>(state.get())) {
                cPrintStmt(std::make_unique<PrintStmt>(std::move(p->value)));
            }
        }
    }

    void Compiler::printI() {
        for(auto &el : instucts) {
            if(auto v = dynamic_cast<PUSH*>(el.get())) {
                cout << "PUSH ";
                if(v->value_int.has_value()) {
                    cout << *v->value_int << endl;
                    continue;
                }
                cout << *v->value_str << endl;
            }
            else if(auto v = dynamic_cast<STORE*>(el.get())) {
                cout << "STORE " << v->name << endl;
            }
            else if(auto v = dynamic_cast<LOAD*>(el.get())) {
                cout << "LOAD " << v->name << endl;
            }
            else if(auto v = dynamic_cast<PRINT*>(el.get())) {
                cout << "PRINT" << endl;
            }
            else if(auto v = dynamic_cast<ADD*>(el.get())) {
                cout << "ADD" << endl;
            }
            else if(auto v = dynamic_cast<SUB*>(el.get())) {
                cout << "SUB" << endl;
            }
            else if(auto v = dynamic_cast<MUL*>(el.get())) {
                cout << "MUL" << endl;
            }
            else if(auto v = dynamic_cast<DIV*>(el.get())) {
                cout << "DIV" << endl;
            }
        }
    }

    void Compiler::SetLetStmt(std::unique_ptr<LetStmt> state) {
        push(std::move(state->value));
        store(state->name);
    }
    void Compiler::cPrintStmt(std::unique_ptr<PrintStmt> state) {
        push(std::move(state->value));
        print();
    }
    void Compiler::push(std::unique_ptr<Expr> expr){
        if(auto bin = dynamic_cast<BinaryExpr*>(expr.get())) {
            push(std::unique_ptr<Expr>(std::move(bin->left)));
            push(std::unique_ptr<Expr>(std::move(bin->right)));
            if(bin->op == Operator::Plus) add();
            else if(bin->op == Operator::Minus) sub();
            else if(bin->op == Operator::Multi) mul();
            else if(bin->op == Operator::Slash) div();
        }
        else if(auto n = dynamic_cast<NumberExpr*>(expr.get())) {
            instucts.push_back(std::make_unique<PUSH>(n->value));
        }
        else if(auto s = dynamic_cast<StrExpr*>(expr.get())) {
            instucts.push_back(std::make_unique<PUSH>(s->value));
        }
        else if(auto v = dynamic_cast<VarExpr*>(expr.get())) {
            instucts.push_back(std::make_unique<LOAD>(v->name));
        }
    }
    void Compiler::store(std::string name){
        instucts.push_back(std::make_unique<STORE>(name));
    }
    void Compiler::print(){
        instucts.push_back(std::make_unique<PRINT>());
    }
    void Compiler::add(){
        instucts.push_back(std::make_unique<ADD>());
    }
    void Compiler::sub(){
        instucts.push_back(std::make_unique<SUB>());
    }
    void Compiler::mul(){
        instucts.push_back(std::make_unique<MUL>());
    }
    void Compiler::div(){
        instucts.push_back(std::make_unique<DIV>());
    }
}