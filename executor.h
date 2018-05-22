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
    Val(){};
    Val(int t){
        type = t;
    }
    const string &getName() const {
        return name;
    }
    std::list<int>* getRange() const {
        return range;
    }
    int getType() const {
        return type;
    }
    void setRange(list<int> *range) {
        Val::range = range;
    }

protected:
    string name;
    std::list<int>* range;
    int type;
};

class MatrixVal : public Val{
public:
    MatrixVal(string n, std::list<int>* r, int t, vector<vector<int>> v){
        name = n;
        range = r;
        type = t;
        value = v;
    }
    const vector<vector<int>> getValue() const {
        return value;
    }
    void setValue(const vector<vector<int>> value) {
        MatrixVal::value = value;
    }
private:
    vector<vector<int>> value;
};

class IntVal : public Val{
public:
    IntVal(string n, std::list<int>* r, int t, int v = 0){
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

class Fun{
public:
    Fun(){}
    Fun(string n, Val* r, Block* b, std::list<Val*>* a){
        name = n;
        retVal = r;
        block = b;
        arguments = a;
        numberOfActivity = 0;
        isActive = false;
    }
    const string &getName() const {
        return name;
    }
    Val *getRetVal() const {
        return retVal;
    }
    Block *getBlock() const {
        return block;
    }
    std::list<Val*>* getArguments() const {
        return arguments;
    }
    void setRetVal(Val *retVal) {
        Fun::retVal = retVal;
    }
    int getNumberOfActivity() const {
        return numberOfActivity;
    }

    void setNumberOfActivity(int numberOfActivity) {
        Fun::numberOfActivity = numberOfActivity;
    }
    bool isIsActive() const {
        return isActive;
    }
    void setIsActive(bool isActive) {
        Fun::isActive = isActive;
    }

private:
    string name;
    Val* retVal;
    std::list<Val*>* arguments;
    Block* block;
    int numberOfActivity;
    bool isActive;

};

class executor {
public:
    vector<vector<int>> execute_matrixValue(Value* value, std::list<int> blockNumber);
    int execute_IntValue(Value* value, std::list<int> blockNumber);
    void MainExecute(Parser* parser);
    Val* execute_functionCall(AssigmentOrFunctionCall* function, std::list<int> blockNumber);
    void execute_returnStatement (ReturnStatement* returnStatement, std::list<int> blockNumber );
    void execute_assigmentOrFunctionCall(AssigmentOrFunctionCall* assigment, std::list<int> blockNumber);
    void execute_block(Block* block);
    int execute_formulaInt(Expression* expression, std::list<int> blockNumber);
    std::vector<std::vector<int>> execute_formulaMatrix(Expression* expression, std::list<int> blockNumber);
    void execute_function (Function* function, std::list<int> blockNumber);
    void execute_assigment(AssigmentOrFunctionCall* assigment, std::list<int> blockNumber);
    Val* execute_declaration(Declaration* declaration, std::list<int> blockNumber);
    void execute_ifStatement(IfStatement* ifStatement, std::list<int> blockNumber);
    bool execute_comparision(Comparision * comparision, std::list<int> blockNumber);
    bool execute_printStatement(PrintStatement* , std::list<int> blockNumber);
    bool execute_condition(Condition* condition, std::list<int> blockNumber);
    void execute_whileStatement(WhileStatement* whileStatement, std::list<int> blockNumber);
private:
    std::list<Val*> values;
    std::list<Fun*> functions;
    int numberOfActivity = 0;
};


#endif //LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H
