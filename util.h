//
// Created by Dominik on 23.05.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_UTIL_H
#define LANGUAGE_INTERPRETER_MATRIXC_UTIL_H

#include <cstring>
#include <list>
#include <vector>
#include <fstream>
#include "iostream"

class executor;

struct SemanticException : public std::exception
{
    virtual const char * what() const throw ()
    {
        return "Zgloszono wyjatek\n";
    }
};

struct MyException : public std::exception
{
    virtual const char * what () const throw ()
    {
        return "";
        //return "Zgloszono wyjatek\n";
    }
};


class Val{
public:
    Val(){};
    const std::string getName() const {
        return name;
    }
    std::list<int> getRange() const {
        return range;
    }
    void setRange(std::list<int> range) {
        Val::range = range;
    }
    void setName(const std::string &name) {
        Val::name = name;
    }
    virtual void setValue(Val* v) {}
    virtual void print() {};
    virtual Val* add(Val* v) {}
    virtual Val* sub(Val* v) {}
    virtual Val* div(Val* v) {}
    virtual Val* mul(Val* v) {}
protected:
    std::string name;
    std::list<int> range;
};

class IntVal : public Val{
public:
    IntVal(int v = 0){
        value = v;
    }
    IntVal(std::string n, std::list<int> r, int v = 0){
        name = n;
        range = r;
        value = v;
    }
    int getValue() const {
        return value;
    }
    void setValue(Val* v) {
        IntVal::value = static_cast<IntVal*>(v)->getValue();
    }
    void print(){
        std::cout << value << std::endl << std::endl;
    }

    virtual Val* add(Val* v) {
        int result = value + static_cast<IntVal*>(v)->getValue();
        return new IntVal(result);
    }
    virtual Val* sub(Val* v) {
        int result = value - static_cast<IntVal*>(v)->getValue();
        return new IntVal(result);
    }
    virtual Val* div(Val* v) {
        int result = value / static_cast<IntVal*>(v)->getValue();
        return new IntVal(result);
    }
    virtual Val* mul(Val* v) {
        int result = value * static_cast<IntVal*>(v)->getValue();
        return new IntVal(result);
    }
private:
    int value;
};

class MatrixVal : public Val{
public:
    MatrixVal(std::vector<std::vector<Val*>> v){
        value = v;
    }
    MatrixVal(std::string n, std::list<int> r, std::vector<std::vector<Val*>> v){
        name = n;
        range = r;
        value = v;
    }
    const std::vector<std::vector<Val*>> getValue() const {
        return value;
    }
    void setValue(Val* v) {
        MatrixVal::value = static_cast<MatrixVal*>(v)->getValue();
    }
    Val* getFromIndex(int x, int y){
        return value[x-1][y-1];
    }
    void print(){
        for(auto col = value.begin(); col != value.end(); col++){
            std::cout << "| ";
            for(auto row = (*col).begin(); row != (*col).end(); row++){
                std::cout << static_cast<IntVal*>(*row)->getValue() << " ";
            }
            std::cout << "|" << std::endl;
        }
        std::cout << std::endl;
    }
    virtual Val* mul(Val* v) {
        if((*(value).begin()).size() != (static_cast<MatrixVal*>(v)->value).size()){
            std::cout << "Blad mnozenia : liczby kolumn prawej macierzy i wierszy lewej macierzy roznia sie!" << std::endl;
            throw MyException();
        }
        auto numberOfOperation = ((static_cast<MatrixVal*>(v)->value)).size();
        auto retNumberOfCol = (*((static_cast<MatrixVal*>(v)->value)).begin()).size();
        auto retNumberOfRow = (*(value).begin()).size();
        Val* insertVal = new Val();
        Val* valToAdd = new Val();
        std::vector<Val*> insertRow;
        std::vector<std::vector<Val*>> insertMatrix;
        for(int i = 0; i < retNumberOfRow; i++){
            insertRow.clear();
            for(int j = 0; j < retNumberOfCol; j++){
                insertVal = new IntVal(0);
                for(int k=0; k < numberOfOperation; k++){
                    auto m = ((static_cast<MatrixVal*>(v)->value))[k][j];
                    auto rm = (value)[i][k];
                    valToAdd = rm->mul(m);
                    insertVal = insertVal->add(valToAdd);
                }
                insertRow.push_back(insertVal);
            }
            insertMatrix.push_back(insertRow);
        }
        return new MatrixVal(insertMatrix);
    }
    virtual Val* sub(Val* v) {
        Val* insertVal = new IntVal(0);
        std::vector<Val*> insertRow;
        std::vector<std::vector<Val*>> insertMatrix;
        auto rows = (value).size();
        auto cols = (*(value).begin()).size();
        if((static_cast<MatrixVal*>(v)->value).size() != rows || (*(static_cast<MatrixVal*>(v)->value).begin()).size() != cols){
            std::cout << "Blad arytmetyczny dodawania!";
            throw MyException();
        }
        for(auto row = 0; row < rows; row++){
            for(auto col = 0; col < cols; col++){
                Val* insertVal = value[row][col]->sub((static_cast<MatrixVal*>(v)->value)[row][col]);
                insertRow.push_back(insertVal);
            }
            insertMatrix.push_back(insertRow);
        }
        return new MatrixVal(insertMatrix);
    }
    virtual Val* div(Val* v) {
        //std::cout << "Dzielenie macierzy jest zabrionione!" << std::end;
    }
    virtual Val* add(Val* v) {
        Val* insertVal = new IntVal(0);
        std::vector<Val*> insertRow;
        std::vector<std::vector<Val*>> insertMatrix;
        auto rows = (value).size();
        auto cols = (*(value).begin()).size();
        if((static_cast<MatrixVal*>(v)->value).size() != rows || (*( static_cast<MatrixVal*>(v)->value).begin()).size() != cols){
            std::cout << "Blad arytmetyczny dodawania!";
            throw MyException();
        }
        for(auto row = 0; row < rows; row++){
            for(auto col = 0; col < cols; col++){
                Val* insertVal = value[row][col]->add((static_cast<MatrixVal*>(v)->value)[row][col]);
                insertRow.push_back(insertVal);
            }
            insertMatrix.push_back(insertRow);
        }
        return new MatrixVal(insertMatrix);
    }
private:
    std::vector<std::vector<Val*>> value;
};



class Value{
public:
    virtual Val* execute(executor* exec){};
};

class Expression{
public:
    Expression(){}
    Expression(std::list<Value*> v, std::list<int> o){
        opers = o;
        vals = v;
    }
    std::list<Value*> getVals() const {
        return vals;
    }

    std::list<int> getOpers() const {
        return opers;
    }
    Val* execute(executor* exec);
private:
    // 0 - '*'     1 - '/'    2 - '+'     3 - '-'
    std::list<Value*> vals;
    std::list<int> opers;
};


class Ident{
public:
    Ident() {}
    virtual Val* execute(executor* exec) {};
    std::string getName() const {
        return name;
    }
protected:
    std::string name;
    std::list<int> blockNumber;
};


class StringIdent : public Ident {
public:
    Val* execute(executor* exec);
    StringIdent(std::string n,std::list<int> b){
        name = n;
        blockNumber = b;
    }
};

class MatrixAccessIdent : public Ident {
public:
    Val* execute(executor* exec);
    MatrixAccessIdent(Expression x, Expression y, std::string n, std::list<int> b){
        x_idenx = x;
        y_idenx = y;
        name = n;
        blockNumber = b;
    }
private:
    Expression x_idenx;
    Expression y_idenx;
};


class IdentValue : public Value{
public:
    Val* execute(executor* exec);
    IdentValue(Ident* v){
        identValue = v;
    }
    Ident* getIdentValue() const {
        return identValue;
    }
private:
    Ident* identValue;
};


class IntValue  : public Value{
public:
    Val* execute(executor* exec);
    IntValue(int v){
        intValue = v;
    }
    int getIntValue() const {
        return intValue;
    }
private:
    int intValue;
};


class MatrixValue  : public Value{
public:
    Val* execute(executor* exec);
    MatrixValue(std::vector<std::vector<Expression>> v){
        matrixValue = v;
    }
    std::vector<std::vector<Expression>> getMatrixValue() const {
        return matrixValue;
    }
private:
    std::vector<std::vector<Expression>> matrixValue;
};

class Statement{
public:
    virtual void execute(executor* exec) {};
    Statement(){};
protected:
    std::list<int> blockNumber;
};

class Declaration : public Statement{
public:
    Declaration(std::list<int> r, int t, std::string n, std::list<int> bN){
        type = t;
        range = r;
        name = n;
        blockNumber = bN;
    }
    int getType() const {
        return type;
    }
    void setType(int type) {
        Declaration::type = type;
    }
    std::list<int> getRange() const {
        return range;
    }
    void setRange(std::list<int> range) {
        Declaration::range = range;
    }
    std::string getName() const {
        return name;
    }
    void setName(std::string name) {
        Declaration::name = name;
    }
    void execute(executor* exec);
private:
    std::list<int> range;
    std::string name; // nazwa
    int type;
};

class AssigmentOrFunctionCall : public Statement{
public:
    AssigmentOrFunctionCall(){}
    virtual void execute(executor* exec){};
protected:
};

class FunctionCall : public AssigmentOrFunctionCall{
public:
    FunctionCall(){};
    FunctionCall(std::list<Expression> arg, std::string n, std::list<int> bn){
        if(arg.size() == 0 ){
            argList;
        }else {
            argList = arg;
        }
        name = n;
        blockNumber = bn;
    }
    std::list<Expression > getArgList() const {
        return argList;
    }
    void setName(const std::string &name) {
        FunctionCall::name = name;
    }
    void execute(executor* exec);
private:
    std::list<Expression> argList;
    std::string name;
};

class FunctionIdent : public Ident {
public:
    FunctionIdent(FunctionCall val, std::string n){
        nameAndArg = val;
        name = n;
    }
    FunctionCall getNameAndArg() const {
        return nameAndArg;
    }
    Val* execute(executor* exec);
private:
    FunctionCall nameAndArg;
};


class Block : public Statement{
public:
    Block(){};
    Block(std::list<Statement*> statements, std::list<int> n){
        statementsBlock = statements;
        number = n;
    }
    std::list<int> getMyNumber() const {
        return number;
    }
    std::list<Statement* > getStatementsBlock() const {
        return statementsBlock;
    }
    Val* getByName(std::string n){
        for(auto iter = listOfValue.begin(); iter != listOfValue.end(); iter++){
            if((*iter)->getName() == n){
                return (*iter);
            }
        }
    }
    void setListOfValue(const std::list<Val *> &listOfValue) {
        Block::listOfValue = listOfValue;
    }
    bool isOnList(std::string name){
        for(auto iter = listOfValue.begin(); iter != listOfValue.end(); iter++){
            if((*iter)->getName() == name){
                return true;
            }
        }
        return false;
    };
    void addValueToList(Val* v){
        listOfValue.push_back(v);
    }
    void assignToVal(Val* val){
        for(auto iter = listOfValue.begin(); iter != listOfValue.end(); iter++){
            if((*iter)->getName() == val->getName()){
                auto name = (*iter)->getName();
                auto range = (*iter)->getRange();
                (*iter) = val;
                (*iter)->setRange(range);
                (*iter)->setName(name);
                break;
            }
        }
    }
    void setValueFromCall(Val* v, std::string n){
        for(auto iter = listOfValue.begin() ; iter != listOfValue.end(); iter++){
           if((*iter)->getName() == n){
               (*iter)->setValue(v);
               break;
           }
        }
    }
    const std::list<Val *> &getListOfValue() const {
        return listOfValue;
    }

    void execute(executor* exec);
private:
    std::list<int> number;
    std::list<Statement*> statementsBlock;
    std::list<Val*> listOfValue;
};


class PrintStatement : public Statement {
public:
    PrintStatement(Expression e, std::list<int> bn) {
        expresionToPrint = e;
        blockNumber = bn;
    }
    Expression getExpresionToPrint() const {
        return expresionToPrint;
    }
    void setExpresionToPrint(Expression expresionToPrint) {
        PrintStatement::expresionToPrint = expresionToPrint;
    }
    void execute(executor* exec);
private:
    Expression expresionToPrint;
    std::list<int> blockNumber;
};


class ReturnStatement : public Statement{
public:
    ReturnStatement(Expression v, std::list<int> bn){
        val = v;
        blockNumber = bn;
    }
    Expression getVal() const {
        return val;
    }
    void execute(executor* exec);
private:
    Expression val;
    std::list<int> blockNumber;
};


class FunctionDeclaration : public Statement{
public:
    FunctionDeclaration(std::list<std::pair<std::string,int>> arg, int ret, Block* b, std::string n, std::list<int> mB, std::list<int> bN){ ;
        arguments = arg;
        returnType = ret;
        block = b;
        name = n;
        myBlocks = mB;
        blockNumber = bN;
    }
    std::list<std::pair<std::string , int > > getArguments() const {
        return arguments;
    }
    void setArguments(std::list<std::pair<std::string , int > > arguments) {
        FunctionDeclaration::arguments = arguments;
    }
    int getReturnType() const {
        return returnType;
    }
    void setReturnType(int returnType) {
        FunctionDeclaration::returnType = returnType;
    }
    Block* getBlock() const {
        return block;
    }
    void setBlock(Block* block) {
        FunctionDeclaration::block = block;
    }
    std::string getName() const {
        return name;
    }
    void setName(std::string name) {
        FunctionDeclaration::name = name;
    }
    std::list<int> getMyBlocks() const {
        return myBlocks;
    }
    void setMyBlocks(std::list<int> myBlocks) {
        FunctionDeclaration::myBlocks = myBlocks;
    }
    void execute(executor* exec);
private:
    std::list<int> myBlocks;
    std::string name;
    int returnType;
    Block* block;
    std::list<std::pair<std::string,int>> arguments;
    std::list<int> blockNumber;
};


class Assigment : public AssigmentOrFunctionCall{
public:
    Assigment() {}
    Assigment(Expression exp, std::string n, std::list<int> bn){
        expression = exp;
        name = n;
        blockNumber = bn;
    }
    Expression getExpression() const {
        return expression;
    }
    void setName(const std::string &name) {
        Assigment::name = name;
    }
    void execute(executor* exec);
private:
    Expression expression;
    std::string name;
};


class Comparision{
public:
    Comparision(int op,Expression le, Expression ri, std::list<int> bn){
        oper = op;
        left = le;
        right = ri;
        blockNumber = bn;
    }
    int getOper() const {
        return oper;
    }
    void setOper(int oper) {
        Comparision::oper = oper;
    }
    Expression getRight() const {
        return right;
    }
    void setRight(Expression right) {
        Comparision::right = right;
    }
    Expression getLeft() const {
        return left;
    }
    void setResult(bool result) {
        Comparision::result = result;
    }
    bool execute(executor* exec);
private:
    Expression right;
    Expression left;
    bool result;
    int oper;
    std::list<int> blockNumber;
};


class Condition{
public:
    Condition(){};
    Condition(std::list<std::pair<Comparision,int>> comparisionAndO, std::list<int> bn){
        comparisionAndOp = comparisionAndO;
    }
    std::list<std::pair<Comparision , int >> getComparisionAndOp() const {
        return comparisionAndOp;
    }
    bool execute(executor* exec);
private:
    std::list<std::pair<Comparision,int>> comparisionAndOp;
    bool result;
    std::list<int> blockNumber;
};


class IfStatement : public Statement{
public:
    IfStatement(Condition cond, Block*b, Block*eB, std::list<int> bn){
        condition = cond;
        block = b;
        elseBlock = eB;
        blockNumber = bn;
    }
    Condition getCondition() const {
        return condition;
    }
    void setCondition(Condition condition) {
        IfStatement::condition = condition;
    }
    Block*getBlock() const {
        return block;
    }
    void setBlock(Block*block) {
        IfStatement::block = block;
    }
    Block*getElseBlock() const {
        return elseBlock;
    }
    void setElseBlock(Block*elseBlock) {
        IfStatement::elseBlock = elseBlock;
    }
    void execute(executor* exec);
private:
    Block*block;
    Block*elseBlock;
    Condition condition;
    std::list<int> blockNumber;
};



class WhileStatement : public Statement{
public:
    WhileStatement(Condition cond, Block*b, std::list<int> bn){
        condition = cond;
        block = b;
        blockNumber = bn;
    }
    Condition getCondition() const {
        return condition;
    }
    void setCondition(Condition condition) {
        WhileStatement::condition = condition;
    }
    Block*getBlock() const {
        return block;
    }
    void setBlock(Block*block) {
        WhileStatement::block = block;
    }
    void execute(executor* exec);
private:
    Block*block;
    Condition condition;
    std::list<int> blockNumber;
};

// ========================================================================================================================================================================================================

class Fun{
public:
    Fun(){}
    Fun(std::string n, Val* r, Block* b, std::list<std::string> a, std::list<int> mB){
        name = n;
        retVal = r;
        block = b;
        numberOfActivity = 0;
        isActive = false;
        argnames = a;
        myBlock = mB;
    }

    const std::string &getName() const {
        return name;
    }
    Val* getRetVal() const {
        return retVal;
    }
    Block* getBlock() const {
        return block;
    }
    void setRetVal(Val* retVal) {
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
    const std::list<std::string> &getArgnames() const {
        return argnames;
    }
    const std::list<int> &getMyBlock() const {
        return myBlock;
    }
private:
    std::string name;
    Val* retVal;
    Block* block;
    int numberOfActivity;
    bool isActive;
    std::list<std::string> argnames;
    std::list<int> myBlock;


};


#endif //LANGUAGE_INTERPRETER_MATRIXC_UTIL_H
