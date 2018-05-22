//
// Created by Dominik on 23.04.2018.
//

#include "parser.h"
#include "list"
#include <exception>
#include <cstring>

Parser::Parser(Scan* scan) {
    this->scan = scan;
}

void Parser::Nexts() {
    lastSymbol = scan->NextSymbol();
}

void Parser::accept(SymType atom) {
    if(lastSymbol==atom) {
        Nexts();
    }
    else {
        SyntaxError(atom);
        throw MyException();
    }
}

void Parser::SyntaxError(int atom) {
    scan->ScanError(FirstSyntaxError+atom,"Spodziewany symbol: ", AT[atom]);
    SyntaxError1(lastSymbol);
}

void Parser::SyntaxError1(int atom) {
    scan->ScanError(FirstSyntaxError+atom,"Nieoczekiwany symbol: ", AT[atom]);
}

bool Parser::Program() {
    Nexts();
    try {
        mainBlock = parse_mainBlock();
        return true;
    }catch (exception& e){
        std::cout << e.what() <<std::endl;
        return false;
    }
}

Parser::~Parser() = default;

FunctionCall* Parser::parse_functionCall() {
    //std::cout << "parsowanie functionCall\n"<<flush;
    std::list<Expression*>* list;
    try {
        accept(lparent);
        list = parse_functionArguments();
        accept(rparent);
        return new FunctionCall(list);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

AssigmentOrFunctionCall* Parser::parse_assigmentOrFunctionCall() {
    //std::cout << "parsowanie assigmentOrFunctionCall\n"<<flush;
    FunctionCall* functionCall;
    Expression* exp;
    string* name;
    try {
        name = parse_identOnlyString();
        switch (lastSymbol) {
            case SymType::becomes :
                exp = parse_assigment();
                accept(semicolon);
                return new AssigmentOrFunctionCall(name,exp);
            default :
                functionCall = parse_functionCall();
                accept(semicolon);
                return new AssigmentOrFunctionCall(name,functionCall);
        }
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Block* Parser::parse_mainBlock() {
    //std::cout << "parsowanie mainBlock\n"<<flush;
    int type;
    auto statementsList = new std::list<Statement*>;
    std::list<int> statementsNumebr;
    currentRangeList.push_back(rangeNumber);
    rangeNumber++;
    auto thisBlockList = currentRangeList;
    try {
        accept(lbracket);
        while (true) {
            switch (lastSymbol) {
                case SymType::functionsy :
                    type = 6;
                    statementsList->push_back(parse_function());
                    break;
                case SymType::privatesy :
                    type = 2;
                    statementsList->push_back(parse_declaration());
                    break;
                case SymType::publicsy :
                    type = 2;
                    statementsList->push_back(parse_declaration());
                    break;
                case SymType::printsy:
                    type = 7;
                    statementsList->push_back(parse_printStatement());
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    return new Block(statementsList,thisBlockList,statementsNumebr);
            }
            statementsNumebr.push_back(type);
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Block*  Parser::parse_functionBlock() {
    //std::cout << "parsowanie functionBlock\n"<<flush;
    int type;
    currentRangeList.push_back(rangeNumber);
    std::list<int> statementsNumebr;
    rangeNumber++;
    auto thisBlockList = currentRangeList;
    auto statementsList = new std::list<Statement*>;
    try {
        accept(lbracket);
        while (true) {
            switch (lastSymbol) {
                case SymType::ident:
                    type = 1;
                    statementsList->push_back(parse_assigmentOrFunctionCall());
                    break;
                case SymType::publicsy :
                    type = 2;
                    statementsList->push_back(parse_declaration());
                    break;
                case SymType::privatesy :
                    type = 2;
                    statementsList->push_back(parse_declaration());
                    break;
                case SymType::ifsy:
                    type = 3;
                    statementsList->push_back(parse_ifStatement());
                    break;
                case SymType::whilesy:
                    type = 4;
                    statementsList->push_back(parse_whileStatement());
                    break;
                case SymType::returnsy:
                    type = 5;
                    statementsList->push_back(parse_returnStatement());
                    break;
                case SymType::printsy:
                    type = 7;
                    statementsList->push_back(parse_printStatement());
                    break;
                case SymType::lbracket :
                    type = 0;
                    statementsList->push_back(parse_block());
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    return new Block(statementsList,thisBlockList,statementsNumebr);
            }
            statementsNumebr.push_back(type);
        }
    }catch (exception& e){
        delete statementsList;
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

PrintStatement* Parser::parse_printStatement(){
    //std::cout << "parsowanie printStatement\n"<<flush;
    try {
        accept(printsy);
        auto expresionToPrint = parse_formula();
        accept(semicolon);
        return new PrintStatement(expresionToPrint);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Function* Parser::parse_function() {
    //std::cout << "parsowanie function\n"<<flush;
    try {
        accept(functionsy);
        auto name = parse_identOnlyString();
        accept(lparent);
        std::list<std::pair<std::string*,int*>*>* list = parse_arguments();
        accept(rparent);
        accept(colon);
        auto type = parse_type();
        auto block = parse_functionBlock();
        return new Function(list,type, block,name);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


std::list<Expression*>* Parser::parse_functionArguments() {  // argumenty podawane przy wołaniu funkcji
    //std::cout << "parsowanie functionArguments\n"<<flush;
    auto list = new std::list<Expression*>;
    try {
        while (true){
            switch(lastSymbol) {
                case SymType::ident:
                    list->push_back(parse_formula());
                    break;
                case SymType::intconst:
                    list->push_back(parse_formula());
                    break;
                case SymType::rparent :
                    return list;
                default:
                    throw MyException();
            }
            if(!(lastSymbol == rparent)){
                accept(comma);
            }
        }
    }catch (exception& e){
        delete list;
        std::cout << e.what() <<std::endl; throw MyException();
    }

}


ReturnStatement* Parser::parse_returnStatement() {
    //std::cout << "parsowanie returnStatement\n"<<flush;
    Expression* expression;
    try {
        accept(returnsy);
        switch (lastSymbol) {
            case SymType::intconst :
                expression = parse_formula();
                accept(semicolon);
                return new ReturnStatement(expression);
            default:
                expression = parse_formula();
                accept(semicolon);
                return new ReturnStatement(expression);
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

WhileStatement* Parser::parse_whileStatement() {
    //std::cout << "parsowanie whileStatement\n"<<flush;
    try {
        accept(whilesy);
        accept(lparent);
        auto con = parse_condition();
        accept(rparent);
        auto block = parse_block();
        return new WhileStatement(con,block);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


IfStatement* Parser::parse_ifStatement() {
    //std::cout << "parsowanie ifStatement\n"<<flush;
    try {
        accept(ifsy);
        accept(lparent);
        auto con = parse_condition();
        accept(rparent);
        auto block = parse_block();
        accept(elsesy);
        auto elseBlock = parse_block();
        return new IfStatement(con,block,elseBlock);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Block* Parser::parse_block() {
    //std::cout << "parsowanie block\n"<<flush;
    currentRangeList.push_back(rangeNumber);
    rangeNumber++;
    auto thisBlockList = currentRangeList;
    std::list<int> statementsNumebr;
    auto statementsList = new std::list<Statement*>;
    try {
        int type;
        accept(lbracket);
        while (true) {
            switch (lastSymbol) {
                case SymType::publicsy :
                    type = 2;
                    statementsList->push_back(parse_declaration());
                    break;
                case SymType::privatesy :
                    type = 2;
                    statementsList->push_back(parse_declaration());
                    break;
                case SymType::ifsy:
                    type = 3;
                    statementsList->push_back(parse_ifStatement());
                    break;
                case SymType::whilesy:
                    type = 4;
                    statementsList->push_back(parse_whileStatement());
                    break;
                case SymType::lbracket :
                    type = 0;
                    statementsList->push_back(parse_block());
                    break;
                case SymType::ident:
                    type = 1;
                    statementsList->push_back(parse_assigmentOrFunctionCall());
                    break;
                case SymType::printsy:
                    type = 7;
                    statementsList->push_back(parse_printStatement());
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    return new Block(statementsList,thisBlockList,statementsNumebr);
            }
            statementsNumebr.push_back(type);
        }
    }catch (exception& e){
        delete statementsList;
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Condition* Parser::parse_condition() {
    //std::cout << "parsowanie condition\n"<<flush;
    auto list = new std::list<std::pair<Comparision*,int*>>;
    try {
        while (true){
            Comparision* comparision = parse_comparision();
            switch (lastSymbol) {
                case SymType::andop:
                    accept(andop);
                    list->push_back(make_pair(comparision,new int(0)));
                    break;
                case SymType::orop:
                    accept(orop);
                    list->push_back(make_pair(comparision,new int(1)));
                    break;
                default :
                    list->push_back(make_pair(comparision,new int(2)));
                    return new Condition(list);
            }
        }
    }catch (exception& e){
        delete list;
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Comparision* Parser::parse_comparision() {
    //std::cout << "parsowanie comparision\n"<<flush;
    auto oper = new int;
    Expression* left;
    Expression* right;
    try {
        left = parse_formula();
        switch (lastSymbol) {
            case SymType::eqop :
                *oper = 0;
                accept(eqop);
                break;
            case SymType::neop :
                *oper = 1;
                accept(neop);
                break;
            case SymType::leop :
                *oper = 2;
                accept(leop);
                break;
            case SymType::ltop :
                *oper = 3;
                accept(ltop);
                break;
            case SymType::geop :
                *oper = 4;
                accept(geop);
                break;
            case SymType::gtop :
                *oper = 5;
                accept(gtop);
                break;
            default:
                break;
        }
        right = parse_formula();
        return new Comparision(oper,left,right);
    }catch (exception& e){
        delete oper;
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Expression* Parser::parse_assigment() {
    //std::cout << "parsowanie assigment\n"<<flush;
    try {
        accept(becomes);
        Expression* expr = parse_formula();
        return expr;
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


Expression* Parser::parse_formula() {
    //std::cout << "parsowanie formula\n"<<flush;
    auto vals = new std::list<Value*>;
    auto opers = new std::list<int>;
    try {
        vals->push_back(parse_value());
        while(true) {
            switch (lastSymbol) {
                case SymType::plusop :
                    accept(plusop);
                    opers->push_back(2);
                    vals->push_back(parse_value());
                    break;
                case SymType::minusop :
                    accept(minusop);
                    opers->push_back(3);
                    vals->push_back(parse_value());
                    break;
                case SymType::times :
                    accept(times);
                    opers->push_back(0);
                    vals->push_back(parse_value());
                    break;
                case SymType::divop:
                    accept(divop);
                    opers->push_back(1);
                    vals->push_back(parse_value());
                    break;
                default:
                    return new Expression(vals,opers);
            }
        }
    }catch (exception & e){
        delete vals;
        delete opers;
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

std::list<std::pair<std::string*,int*>*>* Parser::parse_arguments() { // argumenty podawane przy tworzeniu funkcji
    //std::cout << "parsowanie arguments\n"<<flush;
    std::pair<std::string*,int*>* pair;
    int* type;
    string* name;
    auto argList = new std::list<std::pair<std::string*,int*>*>;
    try {
        while (true){
            if(lastSymbol == rparent){
                break;
            }
            type = parse_type();
            name = parse_identOnlyString();
            pair = new std::pair<std::string*,int*>(name,type);
            argList->push_back(pair);
            if (lastSymbol == SymType::comma) {
                accept(comma);
                continue;
            }else{
                break;
            }
        }
    }catch (exception& e){
        delete argList;
        std::cout << e.what() <<std::endl; throw MyException();
    }
    return argList;
}

Ident* Parser::parse_ident() {
    //std::cout << "parsowanie ident\n"<<flush;
    auto id = new string(scan->Spell());
    try {
        accept(ident);
        if (lastSymbol == laccessop) {
            std::pair<Expression *, Expression *> *pos = parse_matrixPosition();
            return new Ident(pos, id);
        } else {
            if (lastSymbol == lparent) {
                FunctionCall *val = parse_functionCall();
                return new Ident(val, id);
            }
        }
        return new Ident(id);
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Value* Parser::parse_value() {
    //std::cout << "parsowanie value\n"<<flush;
    try {
        switch (lastSymbol) {
            case SymType::intconst :
                return new Value(parse_intconst());
            case SymType::laccessop :
                return new Value(parse_matrixValue());
            default :
                return new Value(parse_ident());
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

std::vector<std::vector<Expression*>*>* Parser::parse_matrixValue() {
    //std::cout << "parsowanie matrixValue\n"<<flush;
    auto vec = new std::vector<std::vector<Expression*>*>;
    try {
        accept(laccessop);
        while(true){
            switch(lastSymbol){
                case SymType::raccessop :
                    accept(raccessop);
                    return vec;
                default:
                    vec->push_back(parse_matrixRow());
                    break;
            }
        }
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
    return vec;
}

std::vector<Expression*>* Parser::parse_matrixRow() {
    //std::cout << "parsowanie matrixRow\n"<<flush;
    auto vec = new std::vector<Expression*>;
    try {
        while(true){
            switch(lastSymbol){
                case SymType::raccessop :
                    return vec;
                case SymType::colon:
                    accept(colon);
                    return vec;
                case SymType::comma:
                    accept(comma);
                    break;
                default:
                    vec->push_back(parse_formula());
                    break;
            }
        }
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
    return vec;
}

std::pair<Expression*,Expression*>* Parser::parse_matrixPosition() {
    //std::cout << "parsowanie matrixPosition\n"<<flush;
    auto p = new std::pair<Expression*,Expression*>;
    try {
        accept(laccessop);
        p->first = parse_formula();
        accept(comma);
        p->second = parse_formula();
        accept(raccessop);
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
    return p;
}

Declaration* Parser::parse_declaration() {
    //std::cout << "parsowanie declaration\n"<<flush;
    auto range = new std::list<int>;
    try {
        for (auto iter = currentRangeList.begin(); iter != currentRangeList.end(); iter++) {
            range->push_back(*iter);
        }
        auto type = new int;
        auto name = new string;
        try {
            switch (lastSymbol) {
                case SymType::publicsy :
                    range->clear();
                    range->push_back(0);
                    accept(publicsy);
                    break;
                default :
                    accept(privatesy);
                    break;
            }
            type = parse_type();
            name = parse_identOnlyString();
            accept(semicolon);
        } catch (exception &e) {
            std::cout << e.what() <<std::endl; throw MyException();
        }
        return new Declaration(type, range, name);
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


int* Parser::parse_type() {
    //std::cout << "parsowanie type\n"<<flush;
    try {
        switch (lastSymbol) {
            case SymType::intsy :
                accept(intsy);
                return new int(0);
            default :
                accept(matrixsy);
                return new int(1);
        }
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

string* Parser::parse_identOnlyString() {
    //std::cout << "parsowanie identOnlyString\n"<<flush;
    try {
        string* id = new string(scan->Spell());
        accept(ident);
        return id;
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

int* Parser::parse_intconst() {
    //std::cout << "parsowanie intconst\n"<<flush;
    try {
        auto c = new int(scan->IntConst());
        accept(intconst);
        return c;
    }catch(exception& e){
    std::cout << e.what() <<std::endl; throw MyException();
}
}


Block *Parser::getMainBlock() const {
    return mainBlock;
}
