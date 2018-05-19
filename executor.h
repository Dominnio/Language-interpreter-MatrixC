//
// Created by Dominik on 09.05.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H
#define LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H

#include <cstring>
#include <list>
#include "parser.h"
#include <fstream>

class Val{
public:
    const string &getName() const {
        return name;
    }
    int getRange() const {
        return range;
    }
    int getType() const {
        return type;
    }
protected:
    string name;
    int range;
    int type;
};

class MatrixVal : public Val{
public:
    MatrixVal(string n, int r, int t, vector<vector<int>> v){
        name = n;
        range = r;
        type = t;
        value = v;
    }
    const vector<vector<int>> &getValue() const {
        return value;
    }
    void setValue(const vector<vector<int>> &value) {
        MatrixVal::value = value;
    }
private:
    vector<vector<int>> value;
};

class IntVal : public Val{
public:
    IntVal(string n, int r, int t, int v = 0){
        name = n;
        range = r;
        type = t;
        value = v;
    }
    int getValue() const {
        return value;
    }
    void setValue(int value) {
        IntVal::value = value;
    }
private:
    int value;
};

class executor {
public:
    void MainExecute(Parser* parser);
    void execute_functionBlock();
    void execute_functionCall();
    void execute_returnStatement ();
    void execute_assigmentOrFunctionCall();
    void execute_functionArguments();
    void execute_arguments();
    void execute_matrixValue();
    void execute_matrixRow();
    void execute_matrixPosition();
    void execute_value();
    void execute_block(Block* block);
    int execute_expressionHighInt(Expression* expression);
    std::vector<std::vector<Expression*>*> execute_expressionHighMatrix(Expression* expression);
    void execute_formula(Expression* expression);
    int execute_formulaInt(Expression* expression);
    std::vector<std::vector<int>> execute_formulaMatrix(Expression* expression);
    void execure_ident();
    void execute_function (Function* function);

    // to juz jest w 100% dobrze
    void execute_assigment(AssigmentOrFunctionCall* assigment, int blockNumber);
    void execute_declaration(Declaration* declaration, int blockNumber);
    void execute_ifStatement(IfStatement* ifStatement);
    bool execute_comparision(Comparision * comparision);
    bool execute_condition(Condition* condition);
    void execute_whileStatement(WhileStatement* whileStatement);
private:
    std::list<Val> values;
};


#endif //LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H
