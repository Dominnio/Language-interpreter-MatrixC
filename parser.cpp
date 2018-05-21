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

void Parser::SemanticError(int ecode) {

}

void Parser::Program() {
    Nexts();
    mainBlock = parse_mainBlock();
}

Parser::~Parser() = default;
/*
 * mainBlock to blok od którego rozpoczynamy parsowanie
 * Tylko w tym bloku możemy deklarować funkcje (nigdzie indziej)
 * W tym bloku możemy zadelkarować zmienne
 * Nie możemy w tym bloku tworzyć pętli, warunków, czy też innych bloków
 * W tym bloku musi znaleźć się funkcja main od której rozpocznie się wykonanie programu.
 */



FunctionCall* Parser::parse_functionCall() {
    //std::cout << "parsowanie functionCall\n"<<flush;
    try {
        accept(lparent);
        std::list<Expression*>* list = parse_functionArguments();
        accept(rparent);
        return new FunctionCall(list);
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}

AssigmentOrFunctionCall* Parser::parse_assigmentOrFunctionCall() {
    //std::cout << "parsowanie assigmentOrFunctionCall\n"<<flush;
    try {
        FunctionCall* functionCall;
        Expression* exp;
        string* name = parse_identOnlyString();
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
        std::cout << e.what() << std::endl;
    }
}


Block* Parser::parse_mainBlock() {
    //std::cout << "parsowanie mainBlock\n"<<flush;
    auto statementsList = new std::list<Statement*>;
    std::list<int> statementsNumebr;
    int type;
    currentRangeList.push_back(rangeNumber);
    rangeNumber++;
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
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    return new Block(statementsList,currentRangeList,statementsNumebr);
            }
            statementsNumebr.push_back(type);
        }
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}


Block*  Parser::parse_functionBlock() {
    //std::cout << "parsowanie functionBlock\n"<<flush;
    currentRangeList.push_back(rangeNumber);
    std::list<int> statementsNumebr;
    int type;
    rangeNumber++;
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
                case SymType::lbracket :
                    type = 0;
                    statementsList->push_back(parse_block());
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    return new Block(statementsList,currentRangeList,statementsNumebr);
            }
            statementsNumebr.push_back(type);
        }
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}

Function* Parser::parse_function() {
    //std::cout << "parsowanie function\n"<<flush;
    try {
        accept(functionsy);
        string* name = parse_identOnlyString();
        accept(lparent);
        std::list<std::pair<std::string*,int*>*>* list = parse_arguments();
        accept(rparent);
        accept(colon);
        int* type = parse_type();
        int firstBlock = rangeNumber;
        Block* block = parse_functionBlock();
        int lastBlock = rangeNumber;
        auto myBlock = new std::list<int>;
        for(int i=firstBlock;i<=lastBlock;i++){
            myBlock->push_back(i);
        }
        return new Function(list,type, block,name,myBlock);
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}


std::list<Expression*>* Parser::parse_functionArguments() {  // argumenty podawane przy wołaniu funkcji
    //std::cout << "parsowanie functionArguments\n"<<flush;
    auto list = new std::list<Expression*>;
    try {
        while (true){
            switch (lastSymbol) {
                case SymType::ident:
                    list->push_back(parse_formula());
                    break;
                case SymType::intconst:
                    list->push_back(parse_formula());
                    break;
                case SymType::rparent :
                    return list;
            }
            if(!(lastSymbol == rparent)){
                accept(comma);
            }
        }
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
    return list;
}


ReturnStatement* Parser::parse_returnStatement() {
    //std::cout << "parsowanie returnStatement\n"<<flush;
    Value* value;
    try {
        accept(returnsy);
        switch (lastSymbol) {
            case SymType::intconst :
                value = new Value(parse_intconst());
            default:
                value = parse_value();
        }
        accept(semicolon);
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
    return new ReturnStatement(value);
}

WhileStatement* Parser::parse_whileStatement() {
    //std::cout << "parsowanie whileStatement\n"<<flush;
    try {
        accept(whilesy);
        accept(lparent);
        Condition* con = parse_condition();
        accept(rparent);
        Block* block = parse_block();
        return new WhileStatement(con,block);
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}


IfStatement* Parser::parse_ifStatement() {
    //std::cout << "parsowanie ifStatement\n"<<flush;
    try {
        accept(ifsy);
        accept(lparent);
        Condition* con = parse_condition();
        accept(rparent);
        Block* block = parse_block();
        accept(elsesy);
        Block* elseBlock = parse_block();
        return new IfStatement(con,block,elseBlock);
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}

/*
 * block to zwykły blok (nie blok funkcji (bo musi być return))
 * W tym bloku niemożemy deklarować funkcji
 * W tym bloku możemy zadelkarować zmienne publiczne i prywatne
 * W tym bloku możemy tworzyć while, ify, inne bloki (np. aby ograniczać widok zmiennych)
 */

Block* Parser::parse_block() {
    //std::cout << "parsowanie block\n"<<flush;
    currentRangeList.push_back(rangeNumber);
    rangeNumber++;
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
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    return new Block(statementsList,currentRangeList,statementsNumebr);
            }
            statementsNumebr.push_back(type);
        }
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}

Condition* Parser::parse_condition() {
    //std::cout << "parsowanie condition\n"<<flush;
    std::list<std::pair<Comparision*,int*>>* list = new std::list<std::pair<Comparision*,int*>>;
    try {
        int* type = new int;
        while (true){
            Comparision* comparision = parse_comparision();
            switch (lastSymbol) {
                case SymType::andop:
                    accept(andop);
                    *type = 0;
                    list->push_back(make_pair(comparision,type));
                    break;
                case SymType::orop:
                    accept(orop);
                    *type = 1;
                    list->push_back(make_pair(comparision,type));
                    break;
                default :
                    *type = 2;
                    list->push_back(make_pair(comparision,type));
                    return new Condition(list);
            }
        }
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}

Comparision* Parser::parse_comparision() {
    //std::cout << "parsowanie comparision\n"<<flush;
    int* oper = new int;
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
        std::cout << e.what() << std::endl;
    }
}


Expression* Parser::parse_assigment() {
    //std::cout << "parsowanie assigment\n"<<flush;
    try {
        accept(becomes);
        Expression* expr = parse_formula();
        return expr;
    }catch (exception& e){
        std::cout << e.what() << std::endl;
    }
}


Expression* Parser::parse_formula() {
    //std::cout << "parsowanie formula\n"<<flush;
    try {
        auto vals = new std::list<Value*>;
        vals->push_back(parse_value());
        auto opers = new std::list<int>;
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
        std::cout << e.what() << std::endl;
    }
}
/*
Expression* Parser::parse_expressionHigh() {
    //std::cout << "parsowanie expressionHigh\n"<<flush;
    try {
        Value* left = parse_value();
        int * type = new int;
        Expression* exp = nullptr;
        switch(lastSymbol){
            case SymType::times :
                accept(times);
                *type = 0;
                exp = new Expression(parse_expressionHigh(),type,left);
                break;
            case SymType::divop:
                accept(divop);
                *type = 1;
                exp = new Expression(parse_expressionHigh(),type,left);
                break;
            default:
                return nullptr;
        }
        return exp;
    }catch (exception & e){
        std::cout << e.what() << std::endl;
    }
}
*/

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
        std::cout << e.what() << std::endl;
    }
    return argList;
}

// tutaj trzeba też obsłużyć np. a[4][6] - też identfkator oraz f(123)
Ident* Parser::parse_ident() {
    //std::cout << "parsowanie ident\n"<<flush;
    auto id = new string(scan->Spell());
    accept(ident);
    if(lastSymbol == laccessop){
        std::pair<Value*,Value*>* pos = parse_matrixPosition();
        return new Ident(pos);
    }else{
        if(lastSymbol == lparent){
            FunctionCall* val = parse_functionCall();
            return new Ident(val);
        }
    }
    return new Ident(id);
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
        std::cout << e.what() << std::endl;
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
        std::cout << e.what() << std::endl;
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
        std::cout << e.what() << std::endl;
    }
    return vec;
}

std::pair<Value*,Value*>* Parser::parse_matrixPosition() {
    //std::cout << "parsowanie matrixPosition\n"<<flush;
    auto p = new std::pair<Value*,Value*>;
    try {
        accept(laccessop);
        p->first = parse_value();
        accept(comma);
        p->second = parse_value();
        accept(raccessop);
    }catch (exception & e){
        std::cout << e.what() << std::endl;
    }
    return p;
}

Declaration* Parser::parse_declaration() {
    //std::cout << "parsowanie declaration\n"<<flush;
    auto range = new std::list<int>(currentRangeList);
    auto type = new int;
    auto name  = new string;
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
    }catch (exception & e){
        std::cout << e.what() << std::endl;
    }
    return new Declaration(type,range,name);
}


int* Parser::parse_type() {
    //std::cout << "parsowanie type\n"<<flush;
    switch (lastSymbol) {
        case SymType::intsy :
            accept(intsy);
            return new int(0);
        default :
            accept(matrixsy);
            return new int(1);
    }
}

string* Parser::parse_identOnlyString() {
    //std::cout << "parsowanie identOnlyString\n"<<flush;
    string* id = new string(scan->Spell());
    accept(ident);
    return id;
}

int* Parser::parse_intconst() {
    //std::cout << "parsowanie intconst\n"<<flush;
    auto c = new int(scan->IntConst());
    accept(intconst);
    return c;
}


Block *Parser::getMainBlock() const {
    return mainBlock;
}