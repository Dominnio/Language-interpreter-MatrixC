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

FunctionCall Parser::parse_functionCall() {
    //std::cout << "parsowanie functionCall\n"<<flush;
    try {
        accept(lparent);
        auto list = parse_functionArguments();
        accept(rparent);
        return  FunctionCall(list,"",currentRangeList);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

AssigmentOrFunctionCall* Parser::parse_assigmentOrFunctionCall() {
    //std::cout << "parsowanie assigmentOrFunctionCall\n"<<flush;
    try {
        auto name = parse_identOnlyString();
        Expression exp;
        switch (lastSymbol) {
            case SymType::becomes :
                exp = parse_assigment();
                accept(semicolon);
                return new Assigment(exp,name, currentRangeList);
            default :
                auto functionCall = parse_functionCall();
                functionCall.setName(name);
                accept(semicolon);
                return new FunctionCall(functionCall.getArgList(),name,currentRangeList);
        }
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Block* Parser::parse_mainBlock() {
    //std::cout << "parsowanie mainBlock\n"<<flush;
    std::list<Statement*> statementsList;
    try {
        accept(lbracket);
        currentRangeList.push_back(rangeNumber);
        rangeNumber++;
        auto thisBlockList = currentRangeList;
        while (true) {
            switch (lastSymbol) {
                case SymType::functionsy :
                    statementsList.push_back(new FunctionDeclaration(parse_function()));
                    break;
                case SymType::privatesy :
                    statementsList.push_back(new Declaration(parse_declaration()));
                    break;
                case SymType::publicsy :
                    statementsList.push_back(new Declaration(parse_declaration()));
                    break;
                case SymType::printsy:
                    statementsList.push_back(new PrintStatement(parse_printStatement()));
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    Block* block = new Block(statementsList,thisBlockList);
                    blockList.push_back(block);
                    return block;
            }
        }
    }catch (exception& e){
        for(auto iter = statementsList.begin(); iter!= statementsList.end(); iter++){
            delete (*iter);
        }
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Block* Parser::parse_functionBlock() {
    //std::cout << "parsowanie functionBlock\n"<<flush;
    std::list<Statement*> statementsList;
    try {
        accept(lbracket);
        currentRangeList.push_back(rangeNumber);
        rangeNumber++;
        auto thisBlockList = currentRangeList;
        while (true) {
            switch (lastSymbol) {
                case SymType::ident:
                    statementsList.push_back(parse_assigmentOrFunctionCall());
                    break;
                case SymType::publicsy :
                    statementsList.push_back(new Declaration(parse_declaration()));
                    break;
                case SymType::privatesy :
                    statementsList.push_back(new Declaration(parse_declaration()));
                    break;
                case SymType::ifsy:
                    statementsList.push_back(new IfStatement(parse_ifStatement()));
                    break;
                case SymType::whilesy:
                    statementsList.push_back(new WhileStatement(parse_whileStatement()));
                    break;
                case SymType::returnsy:
                    statementsList.push_back(new ReturnStatement(parse_returnStatement()));
                    break;
                case SymType::printsy:
                    statementsList.push_back(new PrintStatement(parse_printStatement()));
                    break;
                case SymType::lbracket :
                    statementsList.push_back(parse_block());
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    Block* block = new Block(statementsList,thisBlockList);
                    blockList.push_back(block);
                    return block;
            }
        }
    }catch (exception& e){
        for(auto iter = statementsList.begin(); iter!= statementsList.end(); iter++){
            delete (*iter);
        }
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

PrintStatement Parser::parse_printStatement(){
    //std::cout << "parsowanie printStatement\n"<<flush;
    try {
        accept(printsy);
        auto expresionToPrint = parse_formula();
        accept(semicolon);
        return PrintStatement(expresionToPrint,currentRangeList);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

FunctionDeclaration Parser::parse_function() {
    //std::cout << "parsowanie function\n"<<flush;
    try {
        accept(functionsy);
        auto name = parse_identOnlyString();
        accept(lparent);
        auto list = parse_arguments();
        accept(rparent);
        accept(colon);
        auto type = parse_type();
        auto myBlock = currentRangeList;
        myBlock.push_back(rangeNumber);
        auto block = parse_functionBlock();
        return FunctionDeclaration(list, type, block, name, myBlock ,currentRangeList);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


std::list<Expression> Parser::parse_functionArguments() {  // argumenty podawane przy wołaniu funkcji
    //std::cout << "parsowanie functionArguments\n"<<flush;
    std::list<Expression> list;
    try {
        while (true){
            switch(lastSymbol) {
                case SymType::ident:
                    list.push_back(parse_formula());
                    break;
                case SymType::intconst:
                    list.push_back(parse_formula());
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
        std::cout << e.what() <<std::endl; throw MyException();
    }

}


ReturnStatement Parser::parse_returnStatement() {
    //std::cout << "parsowanie returnStatement\n"<<flush;
    Expression expression;
    try {
        accept(returnsy);
        switch (lastSymbol) {
            case SymType::intconst :
                expression = parse_formula();
                accept(semicolon);
                return  ReturnStatement(expression,currentRangeList);
            default:
                expression = parse_formula();
                accept(semicolon);
                return  ReturnStatement(expression,currentRangeList);
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

WhileStatement Parser::parse_whileStatement() {
    //std::cout << "parsowanie whileStatement\n"<<flush;
    try {
        accept(whilesy);
        accept(lparent);
        auto con = parse_condition();
        accept(rparent);
        auto block = parse_block();
        return  WhileStatement(con,block,currentRangeList);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


IfStatement Parser::parse_ifStatement() {
    //std::cout << "parsowanie ifStatement\n"<<flush;
    try {
        accept(ifsy);
        accept(lparent);
        auto con = parse_condition();
        accept(rparent);
        auto block = parse_block();
        accept(elsesy);
        auto elseBlock = parse_block();
        return  IfStatement(con,block,elseBlock,currentRangeList);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Block* Parser::parse_block() {
    //std::cout << "parsowanie block\n"<<flush;
    std::list<Statement*> statementsList ;
    try {
        accept(lbracket);
        currentRangeList.push_back(rangeNumber);
        rangeNumber++;
        auto thisBlockList = currentRangeList;
        while (true) {
            switch (lastSymbol) {
                case SymType::publicsy :
                    statementsList.push_back(new Declaration(parse_declaration()));
                    break;
                case SymType::privatesy :
                    statementsList.push_back(new Declaration(parse_declaration()));
                    break;
                case SymType::ifsy:
                    statementsList.push_back(new IfStatement(parse_ifStatement()));
                    break;
                case SymType::whilesy:
                    statementsList.push_back(new WhileStatement(parse_whileStatement()));
                    break;
                case SymType::lbracket :
                    statementsList.push_back(parse_block());
                    break;
                case SymType::ident:
                    statementsList.push_back(parse_assigmentOrFunctionCall());
                    break;
                case SymType::printsy:
                    statementsList.push_back(new PrintStatement(parse_printStatement()));
                    break;
                default :
                    accept(rbracket);
                    currentRangeList.pop_back();
                    Block* block = new Block(statementsList,thisBlockList);
                    blockList.push_back(block);
                    return block;
            }
        }
    }catch (exception& e){
        for(auto iter = statementsList.begin(); iter!= statementsList.end(); iter++){
            delete (*iter);
        }
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Condition Parser::parse_condition() {
    //std::cout << "parsowanie condition\n"<<flush;
    std::list<std::pair<Comparision,int>> list;
    try {
        while (true){
            Comparision comparision = parse_comparision();
            switch (lastSymbol) {
                case SymType::andop:
                    accept(andop);
                    list.push_back(make_pair(comparision,0));
                    break;
                case SymType::orop:
                    accept(orop);
                    list.push_back(make_pair(comparision,1));
                    break;
                default :
                    list.push_back(make_pair(comparision,2));
                    return  Condition(list,currentRangeList);
            }
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Comparision Parser::parse_comparision() {
    //std::cout << "parsowanie comparision\n"<<flush;
    try {
        int oper;
        Expression left;
        Expression right;
        left = parse_formula();
        switch (lastSymbol) {
            case SymType::eqop :
                oper = 0;
                accept(eqop);
                break;
            case SymType::neop :
                oper = 1;
                accept(neop);
                break;
            case SymType::leop :
                oper = 2;
                accept(leop);
                break;
            case SymType::ltop :
                oper = 3;
                accept(ltop);
                break;
            case SymType::geop :
                oper = 4;
                accept(geop);
                break;
            case SymType::gtop :
                oper = 5;
                accept(gtop);
                break;
            default:
                break;
        }
        right = parse_formula();
        return  Comparision(oper,left,right,currentRangeList);
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Expression Parser::parse_assigment() {
    //std::cout << "parsowanie assigment\n"<<flush;
    try {
        accept(becomes);
        Expression expr = parse_formula();
        return expr;
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


Expression Parser::parse_formula() {
    //std::cout << "parsowanie formula\n"<<flush;
    std::list<Value*> vals;
    std::list<int> opers;
    try {
        vals.push_back(parse_value());
        while(true) {
            switch (lastSymbol) {
                case SymType::plusop :
                    accept(plusop);
                    opers.push_back(2);
                    vals.push_back(parse_value());
                    break;
                case SymType::minusop :
                    accept(minusop);
                    opers.push_back(3);
                    vals.push_back(parse_value());
                    break;
                case SymType::times :
                    accept(times);
                    opers.push_back(0);
                    vals.push_back(parse_value());
                    break;
                case SymType::divop:
                    accept(divop);
                    opers.push_back(1);
                    vals.push_back(parse_value());
                    break;
                default:
                    return Expression(vals,opers);
            }
        }
    }catch (exception & e){
        for(auto iter = vals.begin(); iter != vals.end(); iter++){
            delete *iter;
        }
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

std::list<std::pair<std::string,int>> Parser::parse_arguments() { // argumenty podawane przy tworzeniu funkcji
    //std::cout << "parsowanie arguments\n"<<flush;

    std::list<std::pair<std::string,int>> argList;
    try {
        while (true){
            if(lastSymbol == rparent){
                break;
            }
            auto type = parse_type();
            auto name = parse_identOnlyString();
            auto pair = make_pair(name,type);
            argList.push_back(pair);
            if (lastSymbol == SymType::comma) {
                accept(comma);
                continue;
            }else{
                return argList;
            }
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Ident* Parser::parse_ident() {
    //std::cout << "parsowanie ident\n"<<flush;
    auto id =  string(scan->Spell());
    try {
        accept(ident);
        if (lastSymbol == laccessop) {
            std::pair<Expression, Expression> pos = parse_matrixPosition();
            return new MatrixAccessIdent(pos.first, pos.second, id,currentRangeList);
        } else {
            if (lastSymbol == lparent) {
                return new FunctionIdent(parse_functionCall(), id);
            }
        }
        return new StringIdent(id,currentRangeList);
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

Value* Parser::parse_value() {
    //std::cout << "parsowanie value\n"<<flush;
    try {
        switch (lastSymbol) {
            case SymType::intconst :
                return new IntValue(parse_intconst());
            case SymType::laccessop :
                return new MatrixValue(parse_matrixValue());
            default :
                return new IdentValue(parse_ident());
        }
    }catch (exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

std::vector<std::vector<Expression>> Parser::parse_matrixValue() {
    //std::cout << "parsowanie matrixValue\n"<<flush;
    std::vector<std::vector<Expression>> vec;
    try {
        accept(laccessop);
        while(true){
            switch(lastSymbol){
                case SymType::raccessop :
                    accept(raccessop);
                    return vec;
                default:
                    vec.push_back(parse_matrixRow());
                    break;
            }
        }
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

std::vector<Expression> Parser::parse_matrixRow() {
    //std::cout << "parsowanie matrixRow\n"<<flush;
    std::vector<Expression> vec;
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
                    vec.push_back(parse_formula());
                    break;
            }
        }
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

std::pair<Expression,Expression> Parser::parse_matrixPosition() {
    //std::cout << "parsowanie matrixPosition\n"<<flush;
    std::pair<Expression,Expression> p;
    try {
        accept(laccessop);
        p.first = parse_formula();
        accept(comma);
        p.second = parse_formula();
        accept(raccessop);
    }catch (exception & e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
    return p;
}

Declaration Parser::parse_declaration() {
    //std::cout << "parsowanie declaration\n"<<flush;
    std::list<int> range;
    try {
        for (auto iter = currentRangeList.begin(); iter != currentRangeList.end(); iter++) {
            range.push_back(*iter);
        }
        int type;
        string name;
        try {
            switch (lastSymbol) {
                case SymType::publicsy :
                    range.clear();
                    range.push_back(0);
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
        return Declaration(range, type, name,currentRangeList);
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


int Parser::parse_type() {
    //std::cout << "parsowanie type\n"<<flush;
    try {
        switch (lastSymbol) {
            case SymType::intsy :
                accept(intsy);
                return 0;
            default :
                accept(matrixsy);
                return 1;
        }
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}

string Parser::parse_identOnlyString() {
    //std::cout << "parsowanie identOnlyString\n"<<flush;
    try {
        string id = string(scan->Spell());
        accept(ident);
        return id;
    }catch(exception& e){
        std::cout << e.what() <<std::endl;
        throw MyException();
    }
}

int Parser::parse_intconst() {
    //std::cout << "parsowanie intconst\n"<<flush;
    try {
        accept(intconst);
        return scan->IntConst();
    }catch(exception& e){
        std::cout << e.what() <<std::endl; throw MyException();
    }
}


Block* Parser::getMainBlock()  {
    return mainBlock;
}

std::list<Block*> Parser::getBlockList(){
    return blockList;
}