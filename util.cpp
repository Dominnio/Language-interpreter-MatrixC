//
// Created by Dominik on 24.05.2018.
//

#include "util.h"
#include "executor.h"
#include "iostream"

void ReturnStatement::execute(executor* exec) {
    try {
        int maxNumberOfAcitvity = -1000;
        auto activeFun = exec->functions.begin();
        for (auto iter = exec->functions.begin(); iter != exec->functions.end(); iter++) {
            if ((*iter)->getNumberOfActivity() > maxNumberOfAcitvity && (*iter)->isIsActive()) {
                maxNumberOfAcitvity = (*iter)->getNumberOfActivity();
                activeFun = iter;
            }
        }
        auto retVal = val.execute(exec);
        (*activeFun)->setRetVal(retVal);
    }catch (exception& e){
        throw SemanticException();
    }
}

Val* Expression::execute(executor* exec){
    try{
        auto opersIter = opers.begin();
        auto valsIter = vals.begin();
        std::list<Val*> valsList;
        std::list<int> operList;
        while(opersIter != opers.end()){
            operList.push_back(*opersIter);
            opersIter++;
        }
        while(valsIter != vals.end()){
            valsList.push_back((*valsIter)->execute(exec));
            valsIter++;
        }
        Val* result;

        if(operList.size() == 0){
            Val* ret = *valsList.begin();
            return ret;
        }
        bool isHighOperator = true;
        while(isHighOperator){
            auto valsListIter = valsList.begin();
            auto valsListNextIter = std::next(valsList.begin()) ;
            auto opersListIter = operList.begin();

            while(*opersListIter == 2 || *opersListIter == 3){
                valsListIter++;
                opersListIter++;
                valsListNextIter++;
                if(opersListIter == operList.end()){
                    break;
                }
            }
            if(opersListIter == operList.end()){
                isHighOperator = false;
            }else{
                if(*opersListIter == (int) 0){
                    (*valsListIter) = (*valsListIter)->mul(*valsListNextIter);
                    valsList.erase(valsListNextIter);
                    operList.erase(opersListIter);
                    continue;
                }else{
                    (*valsListIter) = (*valsListIter)->div(*valsListNextIter);
                    valsList.erase(valsListNextIter);
                    operList.erase(opersListIter);
                    continue;
                }
            }
        }
        auto valsListIter = valsList.begin();
        auto opersListIter = operList.begin();
        result = *valsListIter;
        valsListIter++;
        while(opersListIter != operList.end()){
            if(*opersListIter == 2){
                result = result->add(*valsListIter);
            }else{
                result = result->sub(*valsListIter);
            }
            valsListIter++;
            opersListIter++;
        }
        return result;
    }catch (exception& e){
        throw SemanticException();
    }
}

void Assigment::execute(executor* exec) {
    auto blockList = exec->parser->getBlockList();
    std::list<int> range;
    Val* assign;
    // najpierw szukamy tego bloku w ktorym znajduje sie ta zmienna ( w któym jest przypisanie )
    for(auto iter = blockList.begin(); iter!= blockList.end(); iter++){
        bool foundThisBlock = true;
        if(((*iter)->getMyNumber()).size() != blockNumber.size()){
            continue; // to na pewno nie ten blok
        }
        auto blockRange = ((*iter)->getMyNumber());
        auto blockRangeIter = blockRange.begin();
        for(auto valRangeIter = blockNumber.begin(); valRangeIter != blockNumber.end(); valRangeIter++){
            if(*valRangeIter != *blockRangeIter){
                foundThisBlock = false;
                break;
            } else{
                blockRangeIter++;
            }
        }
        if(!foundThisBlock){
            continue; // to na pewno nie ten blok
        }else{
            range = (*iter)->getByName(name)->getRange();
            assign = expression.execute(exec);
            (*iter)->getByName(name)->setValue(assign);
            // znalazłem ten blok w ktorym dokonuje przypisania
            // zmienna w tym bloku ma range
        }
    }

    // teraz szukam bloków które obejmują zasięgime te zmienna
/*
    for(auto iter = blockList.begin(); iter!= blockList.end(); iter++){
        bool foundThisBlock = true;
        if(((*iter)->getMyNumber()).size() < range.size()){
            continue; // to na pewno nie ten blok
        }
        auto blockRange = ((*iter)->getMyNumber());
        auto blockRangeIter = blockRange.begin();
        for(auto valRangeIter = range.begin(); valRangeIter != range.end(); valRangeIter++){
            if(*valRangeIter != *blockRangeIter){
                foundThisBlock = false;
                break;
            } else{
                blockRangeIter++;
            }
        }
        if(!foundThisBlock){
            continue; // to na pewno nie ten blok
        }else{
            (*iter)->assignToVal(assign);
        }
    }
    */
}


Val* StringIdent::execute(executor* exec) {
    auto blockList = exec->parser->getBlockList();
    // najpierw szukamy tego bloku w ktorym znajduje sie ta zmienna
    for(auto iter = blockList.begin(); iter!= blockList.end(); iter++){
        bool foundThisBlock = true;
        if(((*iter)->getMyNumber()).size() != blockNumber.size()){
            continue; // to na pewno nie ten blok
        }
        auto blockRange = ((*iter)->getMyNumber());
        auto blockRangeIter = blockRange.begin();
        for(auto valRangeIter = blockNumber.begin(); valRangeIter != blockNumber.end(); valRangeIter++){
            if(*valRangeIter != *blockRangeIter){
                foundThisBlock = false;
                break;
            } else{
                blockRangeIter++;
            }
        }
        if(!foundThisBlock){
            continue; // to na pewno nie ten blok
        }else{
            return (*iter)->getByName(name);
        }
    }
}


Val* MatrixAccessIdent::execute(executor* exec) {
    IntVal* i = static_cast<IntVal*>(x_idenx.execute(exec));
    IntVal* j = static_cast<IntVal*>(y_idenx.execute(exec));
    MatrixVal* matrix = static_cast<MatrixVal*>(StringIdent(name,blockNumber).execute(exec));
    Val* val =  matrix->getFromIndex(i->getValue(),j->getValue());
    return val;
}

Val* IdentValue::execute(executor* exec) {
    return identValue->execute(exec);
}

Val* FunctionIdent::execute(executor* exec) {
    try{
        auto fun = exec->functions;
        for (auto iter = fun.begin(); iter != fun.end(); iter++) {
            if ((*iter)->getName() == name) {
                FunctionCall callFun(nameAndArg.getArgList(), name, blockNumber);
                callFun.execute(exec);
                (*iter)->getRetVal()->setRange(blockNumber);
                return (*iter)->getRetVal();
            }
        }
    }catch (exception& e){
        throw SemanticException();
    }
}

void FunctionCall::execute(executor* exec) {
    try{
        Fun* fun;
        bool found = false;
        auto funs = exec->functions;
        for(auto iter = funs.begin(); iter != funs.end(); iter++){
            if((*iter)->getName() == name){
                fun = *iter;
                (*iter)->setNumberOfActivity((exec->numberOfActivity)++);
                (*iter)->setIsActive(true);
                found = true;
                break;
            }
        }
        if(!found){
            std::cout << "Wywolano nieistniejaca funkcje!" << std::endl;
            throw MyException();
        }
        if(!argList.empty()) {
            auto arg = argList.begin();
            auto argnames = fun->getArgnames().begin();
            auto blockList = exec->parser->getBlockList();
            auto blockNumberOfMyFunction = fun->getMyBlock().begin();
            for(auto iter = blockList.begin(); iter != blockList.end(); iter++){
                auto thisBlock = (*iter);
                auto blockNumberList = thisBlock->getMyNumber();
                auto blockNumber = blockNumberList.begin();
                bool isChildren = true;
                if(blockNumberList.size() < fun->getMyBlock().size()){
                    continue;
                }
                for(blockNumberOfMyFunction = fun->getMyBlock().begin(); blockNumberOfMyFunction != fun->getMyBlock().end(); blockNumberOfMyFunction++){
                    if(*blockNumberOfMyFunction != *blockNumber){
                        isChildren = false;
                        break;
                    }else{
                        blockNumber++;
                    }
                }
                if(isChildren){
                    argnames = fun->getArgnames().begin();
                    arg = argList.begin();
                    while (arg != argList.end()) {
                        Val* value = (*arg).execute(exec);
                        (*iter)->setValueFromCall(value,(*argnames));
                        arg++;
                        argnames++;
                    }
                    break;
                }
            }
        }
        ((fun)->getBlock())->execute(exec);
        exec->numberOfActivity--;
        fun->setIsActive(false);
    }catch (exception& e){
        throw SemanticException();
    }
}

void FunctionDeclaration::execute(executor* exec) {
    try{
        for(auto iter = exec->functions.begin(); iter != exec->functions.end(); iter++) {
            if ((*iter)->getName() == name) {       // jeśli funckja o danej nazwie już istnieje
                std::cout << "Deklaracja funkcji o nazwie juz isteniejacej!" << std::endl << std::flush;
                throw MyException();
            }
        }
        // deklarujemy nowe zmienne, ktore beda widoczne tylko w tej funkcji (blokach które należą do tej funkcji)
        std::list<std::string> argnames;
        for(auto arg = arguments.begin(); arg != arguments.end(); arg++){
            argnames.push_back((*arg).first);
            Declaration declaration(myBlocks,(*arg).second,(*arg).first,myBlocks);
            declaration.execute(exec);
        }
        auto arguments = new std::list<Val*>;
        Val* returnVal;
        if(returnType == 0){
            returnVal = new IntVal("",blockNumber,0);
        }else{
            std::vector<std::vector<Val*>> emptyVec;
            returnVal = new MatrixVal("", blockNumber, emptyVec);
        }
        exec->functions.push_back(new Fun(name,returnVal,block, argnames, myBlocks));
    }catch (exception& e){
        throw SemanticException();
    }
}


bool Comparision::execute(executor* exec) {
    try{
        int l = static_cast<IntVal*>(left.execute(exec))->getValue();
        int r = static_cast<IntVal*>(right.execute(exec))->getValue();
        if(oper == 0){
            if(l == r){
                return true;
            }
        }
        if(oper == 1){
            if(l != r){
                return true;
            }
        }
        if(oper == 2){
            if(l <= r){
                return true;
            }
        }
        if(oper == 3){
            if(l < r){
                return true;
            }
        }
        if(oper == 4){
            if(l >= r){
                return true;
            }
        }
        if(oper == 5){
            if(l > r){
                return true;
            }
        }
        return false;
    }catch (std::exception& e){
        throw SemanticException();
    }
}


bool Condition::execute(executor* exec) {
    try{
        if(comparisionAndOp.size() == 1){
            return (*comparisionAndOp.begin()).first.execute(exec);
        }
        bool result;
        auto oper = (*comparisionAndOp.begin()).second;
        for(auto iter = comparisionAndOp.begin(); iter!= comparisionAndOp.end();){
            if((*iter).second != 2){
                if(oper != (*iter).second){
                    std::cout << "Nie można stowsować różnych operacji logicznych w jednym warunku!" << std::endl;
                    throw MyException();
                }
                if((*iter).second == 0){
                    result = result && (*iter).first.execute(exec) && (*iter++).first.execute(exec);
                }else{
                    result = result || (*iter).first.execute(exec) || (*iter++).first.execute(exec);
                }
            }else{
                return result;
            }
        }
    }catch (std::exception& e){
        throw SemanticException();
    }
}


void executor::MainExecute(Parser* parser){
    try{
        this->parser = parser;
        auto block = parser->getMainBlock()->getStatementsBlock();
        while(!block.empty()){
            (*block.begin())->execute(this);
            block.pop_front();
        }
        Fun* mainFun;
        for(auto iter = functions.begin(); iter != functions.end(); iter++){
            if((*iter)->getName() == "main"){
                mainFun = *iter;
                break;
            }
        }
        std::list<Expression> arguments;
        std::list<int> startList;
        startList.push_back(0);
        FunctionCall mainCall(arguments, mainFun->getName(),startList);
        mainCall.execute(this);
    }catch (exception& e){
        throw SemanticException();
    }
}


void IfStatement::execute(executor* exec) {
    try {
        if (condition.execute(exec)) {
            block->execute(exec);
        } else {
            elseBlock->execute(exec);
        }
    } catch (std::exception &e) {

        throw SemanticException();
    }
}


void WhileStatement::execute(executor* exec) {
    try{
        while(condition.execute(exec)){
            block->execute(exec);
        }
    }catch (std::exception& e){

        throw SemanticException();
    }
}

Val* IntValue::execute(executor* exec) {
    return new IntVal(intValue);
}

Val* MatrixValue::execute(executor* exec) {
    vector<vector<Val*>> allVal;
    vector<Val*> rowVal;
    auto width = (*(matrixValue.begin())).size();
    for (auto iter = matrixValue.begin(); iter != matrixValue.end(); iter++) {
        if ((*iter).size() != width) {
            std::cout << "Wiersze macierzy roznia sie liczba elementow!";
            throw MyException();
        }
    }
    for (auto row = matrixValue.begin(); row != matrixValue.end(); row++) {
        rowVal.clear();
        for (auto col = (*row).begin(); col != (*row).end(); col++) {
            rowVal.push_back((*col).execute(exec));
        }
        allVal.push_back(rowVal);
    }
    return new MatrixVal(allVal);
}

void Block::execute(executor* exec) {
    try{
        for(auto iter = statementsBlock.begin(); iter != statementsBlock.end(); iter++){
            (*iter)->execute(exec);
        }
    }catch (std::exception& e){

        throw SemanticException();
    }
}

void Declaration::execute(executor* exec) {
    try{
        auto blockList = exec->parser->getBlockList();
        Val *insert;
        if(type == 0) {
            insert = new IntVal(name, range, 0);
        }
        else{
            std::vector<std::vector<Val*>> emptyVec;
            insert = new MatrixVal(name, range, emptyVec);
        }

        for(auto iter = blockList.begin(); iter!= blockList.end(); iter++){
            bool isVisible = true;
            if(((*iter)->getMyNumber()).size() < range.size()){
                continue; // brak deklaracji - zmienna niewidoczna
            }
            auto blockRange = ((*iter)->getMyNumber());
            auto blockRangeIter = blockRange.begin();
            for(auto valRangeIter = range.begin(); valRangeIter != range.end(); valRangeIter++){
                if(*valRangeIter != *blockRangeIter){
                    isVisible = false;
                    break;
                } else{
                    blockRangeIter++;
                }
            }
            if(!isVisible){
                continue; // brak deklaracji - zmienna niewidoczna
            }else{
                if((*iter)->isOnList(name)){
                    std::cout << "Deklaracja zmiennej o nazwie juz istniejacej!" << std::endl;
                    throw MyException();
                }else{
                    (*iter)->addValueToList(insert);
                }
            }
        }
        //exec->parser->setBlockList(blockList);
    }catch (exception& e){
        throw SemanticException();
    }
}

void PrintStatement::execute(executor* exec) {
    try{
        auto valueToShow = expresionToPrint.execute(exec);
        valueToShow->print();
    }catch (exception& e){
        throw SemanticException();
    }
}
