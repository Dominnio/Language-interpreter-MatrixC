//
// Created by Dominik on 23.04.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_PARSER_H
#define LANGUAGE_INTERPRETER_MATRIXC_PARSER_H

#include <cstring>
#include <list>
#include "scanner.h"
#include <vector>
#include <map>
//#include "PTree.h"

extern char *AT[];

struct MyException : public exception
{
    virtual const char * what () const throw ()
    {
        return "Zgloszono wyjatek\n";
    }
};

class Value;

class Expression{
public:
    Expression(Expression* r, int* op, Value* l){
        oper = op;
        leftVal = l;
        rightExp = r;
    }
private:
    int* oper;
public:
    int *getOper() const {
        return oper;
    }

    void setOper(int *oper) {
        Expression::oper = oper;
    }

    Value *getLeftVal() const {
        return leftVal;
    }

    void setLeftVal(Value *leftVal) {
        Expression::leftVal = leftVal;
    }

    Expression *getRightExp() const {
        return rightExp;
    }

    void setRightExp(Expression *rightExp) {
        Expression::rightExp = rightExp;
    }

private:
    // 0 - '*'     1 - '/'    2 - '+'     3 - '-'
    int* intValue;
public:
    int *getIntValue() const {
        return intValue;
    }

    void setIntValue(int *intValue) {
        Expression::intValue = intValue;
    }

    vector<vector<Expression *> *> *getMatrixValue() const {
        return matrixValue;
    }

    void setMatrixValue(vector<vector<Expression *> *> *matrixValue) {
        Expression::matrixValue = matrixValue;
    }

private:
    std::vector<std::vector<Expression*>*>* matrixValue;
    Value* leftVal;
    Expression* rightExp;
};

class FunctionCall;

class Ident{
public:
    Ident(string* val){
        name = val;
    }
    Ident(std::pair<Value*,Value*>* val){
        position = val;
    }
    Ident(FunctionCall* val){
        nameAndArg = val;
    }
private:
    int* type;
public:
    int *getType() const {
        return type;
    }

    void setType(int *type) {
        Ident::type = type;
    }

    string *getName() const {
        return name;
    }

    void setName(string *name) {
        Ident::name = name;
    }

    pair<Value *, Value *> *getPosition() const {
        return position;
    }

    void setPosition(pair<Value *, Value *> *position) {
        Ident::position = position;
    }

    FunctionCall *getNameAndArg() const {
        return nameAndArg;
    }

    void setNameAndArg(FunctionCall *nameAndArg) {
        Ident::nameAndArg = nameAndArg;
    }

private:
    // 0 zwykły - ala, 1 macierz - ala[2][3], 2 funckja - ala(1)
    string* name;
    std::pair<Value*,Value*>*  position;
    FunctionCall* nameAndArg;
};

class Value{
public:
    Value(int* v){
        type = new int;
        *type = 0;
        intValue = v;
    }
    Value(Ident* v){
        identValue = v;
    }
    Value(std::vector<std::vector<Expression*>*>* v){
        type = new int;
        *type = 1;
        matrixValue = v;
    }
private:
    int* type;
public:
    int *getType() const {
        return type;
    }

    void setType(int *type) {
        Value::type = type;
    }

    int *getIntValue() const {
        return intValue;
    }

    void setIntValue(int *intValue) {
        Value::intValue = intValue;
    }

    Ident *getIdentValue() const {
        return identValue;
    }

    void setIdentValue(Ident *identValue) {
        Value::identValue = identValue;
    }

    vector<vector<Expression *> *> *getMatrixValue() const {
        return matrixValue;
    }

    void setMatrixValue(vector<vector<Expression *> *> *matrixValue) {
        Value::matrixValue = matrixValue;
    }

private:
    // może być trzech typów
    int* intValue;       // albo poprzez po prostu warotść np. 10
    Ident* identValue;           // albo poprzez identyfikator na trzy sposoby np. 'ola' 'ola[1,1]', ola(12)
    std::vector<std::vector<Expression*>*>* matrixValue; // albo poprzez wartość macierzową np. [13,a:b,11,c]
};
class Statement;

class FunctionCall{
public:
    FunctionCall( std::list<Expression*>* arg){
        argList = arg;
    }
private:
    std::list<Expression*>* argList;
public:
    list<Expression *> *getArgList() const {
        return argList;
    }

    void setArgList(list<Expression *> *argList) {
        FunctionCall::argList = argList;
    }
};

class Comparision{
public:
    Comparision(int* op,Expression* le, Expression* ri){
        oper = op;
        left = le;
        right = ri;
    }
private:
    int* oper;
public:
    int *getOper() const {
        return oper;
    }

    void setOper(int *oper) {
        Comparision::oper = oper;
    }

    Expression *getRight() const {
        return right;
    }

    void setRight(Expression *right) {
        Comparision::right = right;
    }

    Expression *getLeft() const {
        return left;
    }

    void setLeft(Expression *left) {
        Comparision::left = left;
    }

    bool *getResult() const {
        return result;
    }

    void setResult(bool *result) {
        Comparision::result = result;
    }

private:
    Expression* right;
    Expression* left;
    bool* result;
};

class Condition{
public:
    Condition(std::list<std::pair<Comparision*,int*>>* comparisionAndO){
        comparisionAndOp = comparisionAndO;
    }
private:
    bool* result;
public:
    bool *getResult() const {
        return result;
    }

    void setResult(bool *result) {
        Condition::result = result;
    }

    list<pair<Comparision *, int *>> *getComparisionAndOp() const {
        return comparisionAndOp;
    }

    void setComparisionAndOp(list<pair<Comparision *, int *>> *comparisionAndOp) {
        Condition::comparisionAndOp = comparisionAndOp;
    }

private:
    std::list<std::pair<Comparision*,int*>>* comparisionAndOp;
};

class Block;
class IfStatement;

class Statement{
public:
    Statement(){};
    int getStatementType() const {
        return statementType;
    }
    void setStatementType(int statementType) {
        Statement::statementType = statementType;
    }
protected:
    int statementType;

};

class ReturnStatement : public Statement{
public:
    ReturnStatement(Value* v ){
        val = v;
    }
private:
    Value* val;
public:
    Value *getVal() const {
        return val;
    }

    void setVal(Value *val) {
        ReturnStatement::val = val;
    }

};

class AssigmentOrFunctionCall : public Statement{
public:
    AssigmentOrFunctionCall(string* n,Expression* a ){
        statementType = 1;
        type = new int;
        *type = 0;
        name= n;
        expression = a;
    }
    AssigmentOrFunctionCall(string* n, FunctionCall* funCall ){
        statementType = 1;
        type = new int;
        *type = 1;
        name = n;
        functionCall = funCall;
    }
private:
    int* type;
public:
    int *getTypeOfConst() const {
        return type;
    }

    void setTypeOfConst(int *type) {
        AssigmentOrFunctionCall::type = type;
    }

    string *getName() const {
        return name;
    }

    void setName(string *name) {
        AssigmentOrFunctionCall::name = name;
    }

    FunctionCall *getFunctionCall() const {
        return functionCall;
    }

    void setFunctionCall(FunctionCall *functionCall) {
        AssigmentOrFunctionCall::functionCall = functionCall;
    }

    Expression *getExpression() const {
        return expression;
    }

    void setAssign(Expression *exp) {
        AssigmentOrFunctionCall::expression = exp;
    }

private:
    string* name;
    FunctionCall* functionCall;
    Expression* expression;


};

class Declaration : public Statement{
public:
    Declaration(int* t, int* r, string* n){
        statementType = 2;
        type = t;
        range = r;
        name = n;
        setStatementType(2);
    }
private:
    int* type;
public:
    int *getType() const {
        return type;
    }

    void setType(int *type) {
        Declaration::type = type;
    }

    int *getRange() const {
        return range;
    }

    void setRange(int *range) {
        Declaration::range = range;
    }

    string *getName() const {
        return name;
    }

    void setName(string *name) {
        Declaration::name = name;
    }

private:
    // 0 - int, 1 - matrix
    int* range; // 0 - public, 1 - priate
    string* name; // nazwa
};

class WhileStatement : public Statement{
public:
    WhileStatement(Condition* cond, Block* b){
        statementType = 4;
        condition = cond;
        block = b;
    }
public:
    Condition *getCondition() const {
        return condition;
    }

    void setCondition(Condition *condition) {
        WhileStatement::condition = condition;
    }

    Block *getBlock() const {
        return block;
    }

    void setBlock(Block *block) {
        WhileStatement::block = block;
    }

private:
    Block* block;
    Condition* condition;
};

class IfStatement : public Statement{
public:
    IfStatement(Condition* cond, Block* b, Block* eB){
        statementType = 3;
        condition = cond;
        block = b;
        elseBlock = eB;
    }

public:
    Condition *getCondition() const {
        return condition;
    }

    void setCondition(Condition *condition) {
        IfStatement::condition = condition;
    }

    Block *getBlock() const {
        return block;
    }

    void setBlock(Block *block) {
        IfStatement::block = block;
    }

    Block *getElseBlock() const {
        return elseBlock;
    }

    void setElseBlock(Block *elseBlock) {
        IfStatement::elseBlock = elseBlock;
    }

private:
    Block* block;
    Block* elseBlock;
    Condition* condition;
};

class Block : public Statement{
public:
    Block(std::list<Statement*>* statements, int n){
        statementType = 0;
        number = n;
        statementsBlock = statements;
    }
public:
    int getNumber() const {
        return number;
    }

    void setNumber(int number) {
        Block::number = number;
    }

    list<Statement *> *getStatementsBlock() const {
        return statementsBlock;
    }

    void setStatementsBlock(list<Statement *> *statementsBlock) {
        Block::statementsBlock = statementsBlock;
    }

private:
    int statementType;
    int number;
    std::list<Statement*>* statementsBlock;
};


class Function : public Statement{
public:
    Function(std::list<std::pair<std::string*,int*>*>* arg, int* ret, Block* b, string* n){
        statementType = 6;
        arguments = arg;
        returnType = ret;
        block = b;
        name = n;
        setStatementType(6);
    }
public:
    list<pair<string *, int *> *> *getArguments() const {
        return arguments;
    }

    void setArguments(list<pair<string *, int *> *> *arguments) {
        Function::arguments = arguments;
    }

    int *getReturnType() const {
        return returnType;
    }

    void setReturnType(int *returnType) {
        Function::returnType = returnType;
    }

    Block *getBlock() const {
        return block;
    }

    void setBlock(Block *block) {
        Function::block = block;
    }

    string *getName() const {
        return name;
    }

    void setName(string *name) {
        Function::name = name;
    }

private:
    string* name;
    int* returnType;
    Block* block;
    std::list<std::pair<std::string*,int*>*>* arguments;
};


class Parser
{
public:
    explicit Parser(Scan* scan);
    ~Parser();
    void Program();
    int getCurrentRange() const;
    void SemanticError(int ecode);
   // const PTree &getMainTree() const;
private:
    Scan* scan;
    int currentRange = 0;
    Block* mainBlock;
public:
    Block *getMainBlock() const;

private:
    SymType lastSymbol; // ostatnio pobrany atom ze scanner'a
    bool can_parse; // jeśli synchronizacja OK
    enum{ FirstSyntaxError=10, FirstSemanticError=60 };

    void Nexts();   // nastepny symbol
    void accept(SymType atom);
    void SyntaxError(int atom);  // oczekiwany atom
    void SyntaxError1(int atom);  // nieoczekiwany atom

    // FUNKCJE ROZBIORU

    Block* parse_mainBlock ();
    Block* parse_block();
    Block* parse_functionBlock();
    Function* parse_function ();
    FunctionCall* parse_functionCall();
    ReturnStatement* parse_returnStatement ();
    Expression* parse_assigment();
    AssigmentOrFunctionCall* parse_assigmentOrFunctionCall();
    std::list<Expression*>* parse_functionArguments();
    WhileStatement* parse_whileStatement();
    IfStatement* parse_ifStatement();
    Condition* parse_condition();
    Comparision* parse_comparision();
    Expression* parse_formula();
    Expression* parse_expressionHigh();
    std::list<std::pair<std::string*,int*>*>* parse_arguments();
    Ident* parse_ident();
    std::vector<std::vector<Expression*>*>* parse_matrixValue();
    std::vector<Expression*>* parse_matrixRow();
    std::pair<Value*,Value*>* parse_matrixPosition();
    Value* parse_value();
    Declaration* parse_declaration();
    int* parse_type();
    string* parse_identOnlyString();
    int* parse_intconst();

};


#endif //LANGUAGE_INTERPRETER_MATRIXC_PARSER_H
