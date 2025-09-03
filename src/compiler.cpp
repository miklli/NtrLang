#include "compiler.h"
#include "utils.h"

namespace ntr {

    ExprType SemanticTable::getVarType(std::string& name) {
        if(STab.find(name) == STab.end()) {
            throw std::runtime_error("Undefined variable: " + name + ".");
        }
        return STab[name];
    }

    void SemanticTable::setVarType(std::string &name, Expr* value) {
        if(STab.find(name) != STab.end()) {
            throw std::runtime_error("The variable '" + name + "' is already initialized."); 
        }
        ExprType etype = getExprType(value, *this);
        STab[name] = etype;
    }


    PUSH::PUSH(int v): value(v) {}
    PUSH::PUSH(std::string v): value(v) {}

    STORE::STORE(std::string name): name(name) {}
    LOAD::LOAD(std::string name): name(name) {}

    Compiler::Compiler(std::vector<std::unique_ptr<Stmt>>& statem): statements(statem) {}
    
    void Compiler::compile(ntr::SemanticTable &STab) {
        SemanticAnalysis(STab);
        for(const auto& state : statements) {
            if(auto l = dynamic_cast<LetStmt*>(state.get())) {
                SetLetStmt(std::make_unique<LetStmt>(l->name, std::move(l->value)));
            }
            else if(auto p = dynamic_cast<PrintStmt*>(state.get())) {
                cPrintStmt(std::make_unique<PrintStmt>(std::move(p->value)));
            }
        }
    }

    void Compiler::SemanticAnalysis(ntr::SemanticTable &STab) {
        for(auto& stmt : statements) {
            if(!isStmtValid(stmt.get(), STab)) {
                throw runtime_error("Unknow error");
            }
        }
    }

    void Compiler::ToBinFile() {
        std::vector<uint8_t> bytecode;
        for(auto &el : instucts) {
            if(auto v = dynamic_cast<PUSH*>(el.get())) {
                bytecode.push_back(v->getOpCode());
                if(auto i = get_if<int>(&v->value)) {
                    IntToBin(bytecode, *i);
                }
                else if(auto s = get_if<std::string>(&v->value)) {
                    StrToBin(bytecode, *s);
                }
            }
            else if(auto v = dynamic_cast<STORE*>(el.get())) {
                bytecode.push_back(v->getOpCode());
                StrToBin(bytecode, v->name);
            }
            else if(auto v = dynamic_cast<LOAD*>(el.get())) {
                bytecode.push_back(v->getOpCode());
                StrToBin(bytecode, v->name);
            }
            else if(auto v = dynamic_cast<Instruct*>(el.get())) {
                bytecode.push_back(v->getOpCode());
            }
        }

        std::ofstream outFile("main.nbc", std::ios::binary);
        if (!outFile) {
            throw runtime_error("Can not compile to '.nbs' file");
        }
        outFile.write(reinterpret_cast<const char*>(bytecode.data()), bytecode.size());
        outFile.close();
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