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
        return "";
        //return "Zgloszono wyjatek\n";
    }
};

class Value;
class FunctionCall;
class Statement;
class Block;

class Expression{
public:
    Expression(std::list<Value*>* v, std::list<int>* o){
        opers = o;
        vals = v;
    }
    list<Value *> *getVals() const {
        return vals;
    }

    list<int> *getOpers() const {
        return opers;
    }
private:
    // 0 - '*'     1 - '/'    2 - '+'     3 - '-'
    std::list<Value*>* vals;
    std::list<int>* opers;
};

class Ident{
public:
    Ident(string* n){
        type = new int(0);
        name = n;
    }
    Ident(std::pair<Expression*,Expression*>* val,string* n){
        type = new int(1);
        position = val;
        name = n;
    }
    Ident(FunctionCall* val, string* n){
        type = new int(2);
        nameAndArg = val;
        name = n;
    }
    int *getType() const {
        return type;
    }
    string *getName() const {
        return name;
    }
    std::pair<Expression*,Expression*> *getPosition() const {
        return position;
    }
    FunctionCall *getNameAndArg() const {
        return nameAndArg;
    }

private:
    // 0 zwykły - ala, 1 macierz - ala[2][3], 2 funckja - ala(1)
    string* name;
    std::pair<Expression*,Expression*>*  position;
    FunctionCall* nameAndArg;
    int* type;

};

class Value{
public:
    Value(int* v){
        type = new int;
        *type = 0;
        intValue = v;
    }
    Value(std::vector<std::vector<Expression*>*>* v){
        type = new int;
        *type = 1;
        matrixValue = v;
    }
    Value(Ident* v){
        type = new int;
        *type = 2;
        identValue = v;
    }
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
    int* type;
};
class FunctionCall{
public:
    FunctionCall(){
        argList = nullptr;
    }
    FunctionCall( std::list<Expression*>* arg){
        if(arg->size() == 0 ){
            argList = nullptr;
        }else {
            argList = arg;
        }
    }
    list<Expression *> *getArgList() const {
        return argList;
    }
private:
    std::list<Expression*>* argList;
};

class Comparision{
public:
    Comparision(int* op,Expression* le, Expression* ri){
        oper = op;
        left = le;
        right = ri;
    }
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
    void setResult(bool *result) {
        Comparision::result = result;
    }
private:
    Expression* right;
    Expression* left;
    bool* result;
    int* oper;
};

class Condition{
public:
    Condition(std::list<std::pair<Comparision*,int*>>* comparisionAndO){
        comparisionAndOp = comparisionAndO;
    }
    list<pair<Comparision *, int *>> *getComparisionAndOp() const {
        return comparisionAndOp;
    }
private:
    std::list<std::pair<Comparision*,int*>>* comparisionAndOp;
    bool* result;
};

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

class PrintStatement : public Statement {
public:
    PrintStatement(Expression *e) {
        statementType = 7;
        expresionToPrint = e;
    }
    Expression *getExpresionToPrint() const {
        return expresionToPrint;
    }
    void setExpresionToPrint(Expression *expresionToPrint) {
        PrintStatement::expresionToPrint = expresionToPrint;
    }
private:
    Expression *expresionToPrint;
};

class ReturnStatement : public Statement{
public:
    ReturnStatement(Expression* v ){
        statementType = 5;
        val = v;
    }
    Expression* getVal() const {
        return val;
    }
private:
    Expression* val;
};

class AssigmentOrFunctionCall : public Statement{
public:
    AssigmentOrFunctionCall(string* n,Expression* a ){
        statementType = 1;
        type = new int(0);
        name= n;
        expression = a;
    }
    AssigmentOrFunctionCall(string* n, FunctionCall* funCall ){
        statementType = 1;
        type = new int(1);
        name = n;
        functionCall = funCall;
    }
public:
    int *getType() const {
        return type;
    }
    void setType(int *type) {
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
    int* type;
};

class Declaration : public Statement{
public:
    Declaration(int* t, std::list<int>* r, string* n){
        statementType = 2;
        type = t;
        range = r;
        name = n;
        setStatementType(2);
    }
    int *getType() const {
        return type;
    }
    void setType(int *type) {
        Declaration::type = type;
    }
    std::list<int> *getRange() const {
        return range;
    }
    void setRange(std::list<int> *range) {
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
    std::list<int>* range;
    string* name; // nazwa
    int* type;
};

class WhileStatement : public Statement{
public:
    WhileStatement(Condition* cond, Block* b){
        statementType = 4;
        condition = cond;
        block = b;
    }
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
    Block(std::list<Statement*>* statements, std::list<int> n, std::list<int> stN){
        statementType = 0;
        number = n;
        statementsNumbers = stN;
        statementsBlock = statements;
    }
    std::list<int> getNumber() const {
        return number;
    }
    void setNumber(std::list<int> number) {
        Block::number = number;
    }
    list<Statement *> *getStatementsBlock() const {
        return statementsBlock;
    }
    void setStatementsBlock(list<Statement *> *statementsBlock) {
        Block::statementsBlock = statementsBlock;
    }
    const list<int> &getStatementsNumbers() const {
        return statementsNumbers;
    }
    void setStatementsNumbers(const list<int> &statementsNumbers) {
        Block::statementsNumbers = statementsNumbers;
    }
private:
    std::list<int> number;
    std::list<int> statementsNumbers;
    std::list<Statement*>* statementsBlock;
};


class Function : public Statement{
public:
    Function(std::list<std::pair<std::string*,int*>*>* arg, int* ret, Block* b, string* n){ ;
        statementType = 6;
        arguments = arg;
        returnType = ret;
        block = b;
        name = n;
        setStatementType(6);
    }
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
    list<int> *getMyBlocks() const {
        return myBlocks;
    }
    void setMyBlocks(list<int> *myBlocks) {
        Function::myBlocks = myBlocks;
    }
private:
    std::list<int>* myBlocks;
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
    bool Program();
    Block *getMainBlock() const;
private:
    Scan* scan;
    std::list<int> currentRangeList;
    int rangeNumber = 0;
    Block* mainBlock;
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
    std::list<std::pair<std::string*,int*>*>* parse_arguments();
    Ident* parse_ident();
    std::vector<std::vector<Expression*>*>* parse_matrixValue();
    std::vector<Expression*>* parse_matrixRow();
    std::pair<Expression*,Expression*>* parse_matrixPosition();
    Value* parse_value();
    Declaration* parse_declaration();
    int* parse_type();
    string* parse_identOnlyString();
    int* parse_intconst();
    PrintStatement* parse_printStatement();

};

#endif //LANGUAGE_INTERPRETER_MATRIXC_PARSER_H
