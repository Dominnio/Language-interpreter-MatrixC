//
// Created by Dominik on 09.05.2018.
//

#include "executor.h"
#include <algorithm>

struct SemanticException : public exception
{
    virtual const char * what() const throw ()
    {
        //return "";
        return "Zgloszono wyjatek\n";
    }
};

void executor::execute_returnStatement(ReturnStatement* returnStatement, std::list<int> blockNumber ) {
    try {
        int maxNumberOfAcitvity = -1000;
        auto activeFun = functions.begin();
        for (auto iter = functions.begin(); iter != functions.end(); iter++) {
            if ((*iter)->getNumberOfActivity() > maxNumberOfAcitvity && (*iter)->isIsActive()) {
                maxNumberOfAcitvity = (*iter)->getNumberOfActivity();
                activeFun = iter;
            }
        }
        if ((*activeFun)->getRetVal()->getType() == 0) {
            auto retVal = new IntVal("functionReturnValue", new list<int>, 0, execute_formulaInt(returnStatement->getVal(), blockNumber));
            (*activeFun)->setRetVal(retVal);
        } else {
            auto retVal = new MatrixVal("functionReturnValue", new list<int>, 1,execute_formulaMatrix(returnStatement->getVal(), blockNumber));
            (*activeFun)->setRetVal(retVal);
            static_cast<MatrixVal*>((*activeFun)->getRetVal())->setValue(execute_formulaMatrix(returnStatement->getVal(), blockNumber));
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}

std::vector<std::vector<int>> executor::execute_formulaMatrix(Expression* expression, std::list<int> blockNumber) {
    try{
        auto opersIter = expression->getOpers()->begin();
        auto valsIter = expression->getVals()->begin();
        auto valsList = new std::list<std::vector<std::vector<int>>>;
        auto operList = new std::list<int>;
        while(opersIter != expression->getOpers()->end()){
            operList->push_back(*opersIter);
            opersIter++;
        }
        while(valsIter != expression->getVals()->end()){
            valsList->push_back(execute_matrixValue(*valsIter,blockNumber));
            valsIter++;
        }
        std::vector<std::vector<int>> result = *(valsList->begin());

        if(operList->size() == 0){
            return result;
        }
        result.clear();
        bool isHighOperator = true;
        while(isHighOperator){
            auto valsListIter = valsList->begin();
            auto valsListNextIter = std::next(valsList->begin()) ;
            auto opersListIter = operList->begin();

            while(*opersListIter == 2 || *opersListIter == 3){
                valsListIter++;
                opersListIter++;
                valsListNextIter++;
                if(opersListIter == operList->end()){
                    break;
                }
            }
            if(opersListIter == operList->end()){
                isHighOperator = false;
            }else{
                if(*opersListIter == (int) 0){

                    if((*(*valsListIter).begin()).size() != (*valsListNextIter).size()){
                        std::cout << "Blad mnozenia : liczby kolumn prawej macierzy i wierszy lewej macierzy roznia sie!" << std::endl;
                        throw MyException();
                    }
                    auto numberOfOperation = (*valsListNextIter).size();
                    auto retNumberOfCol = (*(*valsListNextIter).begin()).size();
                    auto retNumberOfRow = (*valsListIter).size();
                    int insertVal = 0;
                    std::vector<int> insertRow;
                    std::vector<std::vector<int>> insertMatrix;
                    for(int i = 0; i < retNumberOfRow; i++){
                        insertRow.clear();
                        for(int j = 0; j < retNumberOfCol; j++){
                            insertVal = 0;
                            for(int k=0; k < numberOfOperation; k++){
                                insertVal += (*valsListIter)[i][k] * (*valsListNextIter)[k][j];
                            }
                            insertRow.push_back(insertVal);
                        }
                        insertMatrix.push_back(insertRow);
                    }
                    *valsListIter = insertMatrix;
                    valsList->erase(valsListNextIter);
                    operList->erase(opersListIter);
                    continue;
                }else{
                    std::cout << "Dzielenie macierzy jest zabronione!" << std::endl;
                    throw MyException();
                }
            }
        }
        auto valsListIter = valsList->begin();
        auto opersListIter = operList->begin();
        result = *(valsList->begin());
        auto rows = (*(valsList->begin())).size();
        auto cols = (*(*(valsList->begin())).begin()).size();
        valsListIter++;
        while(opersListIter != operList->end()){
            if(*opersListIter == 2){
                if((*valsListIter).size() != rows || (*(*valsListIter).begin()).size() != cols){
                    std::cout << "Blad arytmetyczny dodawania!";
                    throw MyException();
                }
                for(auto row = 0; row < rows; row++){
                    for(auto col = 0; col < cols; col++){
                        result[row][col] += (*valsListIter)[row][col];
                    }
                }
            }else{
                if((*valsListIter).size() != rows || (*(*valsListIter).begin()).size() != cols){
                    std::cout << "Blad arytmetyczny odejmowania!";
                    throw MyException();
                }
                for(auto row = 0; row < rows; row++){
                    for(auto col = 0; col < cols; col++){
                        result[row][col] -= (*valsListIter)[row][col];
                    }
                }
            }
            valsListIter++;
            opersListIter++;
        }
        return result;
    }catch (exception& e){
        
        throw SemanticException();
    }
}

int executor::execute_formulaInt(Expression* expression, std::list<int> blockNumber) {
    try{
        auto opersIter = expression->getOpers()->begin();
        auto valsIter = expression->getVals()->begin();
        auto valsList = new std::list<int>;
        auto operList = new std::list<int>;
        while(opersIter != expression->getOpers()->end()){
            operList->push_back(*opersIter);
            opersIter++;
        }
        while(valsIter != expression->getVals()->end()){
            valsList->push_back(execute_IntValue(*valsIter,blockNumber));
            valsIter++;
        }
        int result = 0;

        if(operList->size() == 0){
            return *valsList->begin();
        }
        bool isHighOperator = true;
        while(isHighOperator){
            auto valsListIter = valsList->begin();
            auto valsListNextIter = std::next(valsList->begin()) ;
            auto opersListIter = operList->begin();

            while(*opersListIter == 2 || *opersListIter == 3){
                valsListIter++;
                opersListIter++;
                valsListNextIter++;
                if(opersListIter == operList->end()){
                    break;
                }
            }
            if(opersListIter == operList->end()){
                isHighOperator = false;
            }else{
                if(*opersListIter == (int) 0){
                    *valsListIter *= *valsListNextIter;
                    valsList->erase(valsListNextIter);
                    operList->erase(opersListIter);
                    continue;
                }else{
                    *valsListIter /= *valsListNextIter;
                    valsList->erase(valsListNextIter);
                    operList->erase(opersListIter);
                    continue;
                }
            }
        }
        auto valsListIter = valsList->begin();
        auto opersListIter = operList->begin();
        result = *valsListIter;
        valsListIter++;
        while(opersListIter != operList->end()){
            if(*opersListIter == 2){
                result += *valsListIter;
            }else{
                result -= *valsListIter;
            }
            valsListIter++;
            opersListIter++;
        }
        return result;
    }catch (exception& e){
        
        throw SemanticException();
    }
}


vector<vector<int>> executor::execute_matrixValue(Value* value, std::list<int> blockNumber) {
    try{
        bool flag = true;
        vector<vector<int>> allVal;
        vector<int> rowVal;
        if (*value->getType() == 2 && *value->getIdentValue()->getType() == 1) {
            std::cout << "Elementem macierzy nie moze byc macierz";
            throw MyException();
        }
        if (*value->getType() == 2 && *value->getIdentValue()->getType() == 2) {
            for (auto iter = functions.begin(); iter != functions.end(); iter++) {
                if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                    return static_cast<MatrixVal*>(execute_functionCall(new AssigmentOrFunctionCall(new string(*value->getIdentValue()->getName()),value->getIdentValue()->getNameAndArg()),(*iter)->getBlock()->getNumber()))->getValue();
                }
            }
        }
        if (*value->getType() == 2 && *value->getIdentValue()->getType() == 0) {
            for (auto iter = values.begin(); iter != values.end(); iter++) {
                if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                    if ((*iter)->getRange()->size() == 1 && *((*iter)->getRange()->begin()) == 0) { // zrob przypisanie bo to zmienna publiczna
                        return static_cast<MatrixVal*>(*iter)->getValue();
                    }
                    if((*iter)->getRange()->size() > blockNumber.size()){    // to na pewno nie jest ta zmienna ktorej szukamy, bo ta nie jest widoczna w tym bloku
                        continue;
                    }
                    auto num2 = blockNumber.begin();
                    for (auto num = (*iter)->getRange()->begin(); num != (*iter)->getRange()->end(); num++) {
                        if (*num != *num2) {
                            flag = false;
                            break; // to nie ta zmienna
                        }
                        num2++;
                    }
                    if(!flag){
                        flag = true;
                        continue;
                    }
                    if((*iter)->getType() == 1 ){
                        return static_cast<MatrixVal*>(*iter)->getValue();
                    }else{
                        // tu jest blad bo jednak sie typy nie zgadzaja
                    }
                }
            }
            std::cout << "Uzycie niezadeklarowanej zmiennej matrix!" << std::endl;
            throw MyException();
            // identyfikator - trzeba siegnac do tablicy w wykonawcy
        }
        if (*value->getType() == 1) {
            auto width = (*value->getMatrixValue()->begin())->size();
            for(auto iter = value->getMatrixValue()->begin(); iter != value->getMatrixValue()->end(); iter++){
                if((*iter)->size() != width){
                    std::cout << "Wiersze macierzy roznia sie liczba elementow!";
                    throw MyException();
                }
            }
            for(auto row = value->getMatrixValue()->begin(); row != value->getMatrixValue()->end(); row++){
                rowVal.clear();
                for(auto col = (*row)->begin(); col != (*row)->end(); col++){
                    rowVal.push_back(execute_formulaInt(*col,blockNumber));
                }
                allVal.push_back(rowVal);
            }
            return allVal;
        }
        if (*value->getType() == 0) {
            std::cout << "Brak wymaganej wartosci matrix!" << std::endl;
            throw MyException();
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}

// tutaj trzeba jeszcze rowzazyc jeden przypadek konkretnie to ident ktory jest taki ala[1,2] oraz ala() jako funkcja i to samo dla matrix
int executor::execute_IntValue(Value* value, std::list<int> blockNumber) {
    try{
        bool flag = true;
        if (*value->getType() == 2 && *value->getIdentValue()->getType() == 2) {
            for (auto iter = functions.begin(); iter != functions.end(); iter++) {
                if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                    return static_cast<IntVal*>(execute_functionCall(new AssigmentOrFunctionCall(new string(*value->getIdentValue()->getName()),value->getIdentValue()->getNameAndArg()),(*iter)->getBlock()->getNumber()))->getValue();
                }
            }
        }
        if (*value->getType() == 2 && *value->getIdentValue()->getType() == 0) {
            for (auto iter = values.begin(); iter != values.end(); iter++) {
                if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                    if ((*iter)->getRange()->size() == 1 && *((*iter)->getRange()->begin()) == 0) { // zrob przypisanie bo to zmienna publiczna
                        return static_cast<IntVal*>(*iter)->getValue();
                    }
                    if((*iter)->getRange()->size() > blockNumber.size()){    // to na pewno nie jest ta zmienna ktorej szukamy, bo ta nie jest widoczna w tym bloku
                        continue;
                    }
                    auto num2 = blockNumber.begin();
                    for (auto num = (*iter)->getRange()->begin(); num != (*iter)->getRange()->end(); num++) {
                        if (*num != *num2) {
                            flag = false;
                            break; // to nie ta zmienna
                        }
                        num2++;
                    }
                    if(!flag){
                        flag = true;
                        continue;
                    }
                    if((*iter)->getType() == 0 ){
                        return static_cast<IntVal*>(*iter)->getValue();
                    }else{
                        return 0;
                        // tu jest blad bo jednak sie typy nie zgadzaja
                    }
                }
            }
            std::cout << "Uzycie niezadeklarowanej zmiennej int!" << std::endl;
            throw MyException();
            // identyfikator - trzeba siegnac do tablicy w wykonawcy
        }
        if (*value->getType() == 2 && *value->getIdentValue()->getType() == 1) {
            for (auto iter = values.begin(); iter != values.end(); iter++) {
                if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                    if ((*iter)->getRange()->size() == 1 && *((*iter)->getRange()->begin()) == 0) { // zrob przypisanie bo to zmienna publiczna
                        if((*iter)->getType() == 0){
                            std::cout << "Bledny typ!" << std::endl;
                            throw MyException();
                        }
                        int i = execute_formulaInt(value->getIdentValue()->getPosition()->first, blockNumber);
                        int j = execute_formulaInt(value->getIdentValue()->getPosition()->second, blockNumber);
                        if(i > static_cast<MatrixVal*>(*iter)->getValue().size() || j > (*(static_cast<MatrixVal*>(*iter)->getValue().begin())).size()){
                            std::cout << "Przekroczono indeks macierzy!" << std::endl;
                            throw MyException();
                        }
                        return static_cast<MatrixVal*>(*iter)->getValue()[i-1][j-1];
                    }
                    if((*iter)->getRange()->size() > blockNumber.size()){    // to na pewno nie jest ta zmienna ktorej szukamy, bo ta nie jest widoczna w tym bloku
                        continue;
                    }
                    auto num2 = blockNumber.begin();
                    for (auto num = (*iter)->getRange()->begin(); num != (*iter)->getRange()->end(); num++) {
                        if (*num != *num2) {
                            flag = false;
                            break; // to nie ta zmienna
                        }
                        num2++;
                    }
                    if(!flag){
                        flag = true;
                        continue;
                    }
                    if((*iter)->getType() == 0){
                        std::cout << "Bledny typ!" << std::endl;
                        throw MyException();
                    }
                    int i = execute_formulaInt(value->getIdentValue()->getPosition()->first, blockNumber);
                    int j = execute_formulaInt(value->getIdentValue()->getPosition()->second, blockNumber);
                    if(i > static_cast<MatrixVal*>(*iter)->getValue().size() || j > (*(static_cast<MatrixVal*>(*iter)->getValue().begin())).size()){
                        std::cout << "Przekroczono indeks macierzy!" << std::endl;
                        throw MyException();
                    }
                    return static_cast<MatrixVal*>(*iter)->getValue()[i-1][j-1];
                }
            }
            std::cout << "Niezadeklarowana zmienna" << std::endl;
            // identyfikator - trzeba siegnac do tablicy w wykonawcy
        }
        if (*value->getType() == 0) {
            return *value->getIntValue();
        }
        if (*value->getType() == 1) {
            std::cout << "Brak wymaganej wartosci int" << std::endl;
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}


// tutaj jest pewna nie zerowa szansa, że jest dobrze

void executor::execute_assigmentOrFunctionCall(AssigmentOrFunctionCall* assigment, std::list<int> blockNumber) {
    try{
        if(*(assigment->getType()) == 0){
            execute_assigment(assigment,blockNumber);
        }else{
            execute_functionCall(assigment,blockNumber);
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}


void executor::MainExecute(Parser* parser){
    try{
        for(auto iter = parser->getMainBlock()->getStatementsBlock()->begin(); iter != parser->getMainBlock()->getStatementsBlock()->end(); iter++){
            if((*iter)->getStatementType() == 2)
            {
                execute_declaration(static_cast<Declaration*>(*iter),parser->getMainBlock()->getNumber());
            }
            if((*iter)->getStatementType() == 6)
            {
                execute_function(static_cast<Function*>(*iter),parser->getMainBlock()->getNumber());
            }
        }
        Fun* mainFun = new Fun();
        for(auto iter = functions.begin(); iter != functions.end(); iter++){
            if((*iter)->getName() == "main"){
                mainFun = *iter;
                break;
            }
        }
        FunctionCall* mainCall = new FunctionCall();

        execute_functionCall(new AssigmentOrFunctionCall(new string("main"),mainCall),parser->getMainBlock()->getNumber());
    }catch (exception& e){
        
        throw SemanticException();
    }
}

Val* executor::execute_functionCall(AssigmentOrFunctionCall* functionCall, std::list<int> blockNumber) {
    try{
        Fun* fun = new Fun();
        bool found = false;
        for(auto iter = functions.begin(); iter != functions.end(); iter++){
            if((*iter)->getName() == *functionCall->getName()){
                fun = *iter;
                (*iter)->setNumberOfActivity(numberOfActivity++);
                (*iter)->setIsActive(true);
                found = true;
                break;
            }
        }
        if(!found){
            std::cout << "Wysolano nieistniejaca funkcje!" << std::endl;
        }
        if(functionCall->getFunctionCall()->getArgList() != nullptr) {
            auto arg = functionCall->getFunctionCall()->getArgList()->begin();
            auto iter = (fun->getArguments()->begin());
            while (arg != functionCall->getFunctionCall()->getArgList()->end()) {
                if ((*iter)->getType() == 0) {
                    static_cast<IntVal*>(*iter)->setValue(execute_formulaInt(*arg, fun->getBlock()->getNumber()));
                } else {
                    static_cast<MatrixVal*>(*iter)->setValue(execute_formulaMatrix(*arg, fun->getBlock()->getNumber()));
                }
                arg++;
                iter++;
            }
        }
        execute_block((fun)->getBlock());
        numberOfActivity--;
        fun->setIsActive(false);
        return fun->getRetVal();
    }catch (exception& e){
        
        throw SemanticException();
    }
}

// to jest wykonanie DEKLARACJI FUNKCJI !!!
void executor::execute_function(Function* function, std::list<int> blockNumber) {
    try{
        for(auto iter = functions.begin(); iter != functions.end(); iter++) {
            if ((*iter)->getName() == *function->getName()) {       // jeśli funckja o danej nazwie już istnieje
                std::cout << "Deklaracja funkcji o nazwie juz isteniejacej!" << std::endl << std::flush;
                throw MyException();
            }
        }
        // deklarujemy nowe zmienne, ktore beda widoczne tylko w tej funkcji
        auto arguments = new std::list<Val*>;
        for(auto arg = function->getArguments()->begin(); arg != function->getArguments()->end(); arg++) {
            Declaration *declaration = new Declaration(new int(*((*arg)->second)), new std::list<int>, new string(*((*arg)->first)));
            arguments->push_back(execute_declaration(declaration,function->getBlock()->getNumber()));
        }
        functions.push_back(new Fun(*function->getName(),new Val(*function->getReturnType()),function->getBlock(),arguments));
    }catch (exception& e){
        
        throw SemanticException();
    }
}


void executor::execute_block(Block* block) {
    try{
        auto num = block->getStatementsNumbers().begin();
        for(auto iter = block->getStatementsBlock()->begin(); iter != block->getStatementsBlock()->end(); iter++){
            if(*num == 0)
            {
                execute_block(static_cast<Block*>(*iter));
            }
            if(*num  == 1)
            {
                execute_assigmentOrFunctionCall(static_cast<AssigmentOrFunctionCall*>(*iter), block->getNumber());
            }
            if(*num  == 2)
            {
                execute_declaration(static_cast<Declaration*>(*iter), block->getNumber());
            }
            if(*num  == 3)
            {
                execute_ifStatement(static_cast<IfStatement*>(*iter), block->getNumber());
            }
            if(*num == 4)
            {
                execute_whileStatement(static_cast<WhileStatement*>(*iter), block->getNumber());
            }
            if(*num  == 5)
            {
                execute_returnStatement(static_cast<ReturnStatement*>(*iter), block->getNumber());
                break;
            }
            if(*num == 7)
            {
                execute_printStatement(static_cast<PrintStatement*>(*iter), block->getNumber());
            }
            num++;
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}
bool executor::execute_printStatement(PrintStatement* print, std::list<int> blockNumber){
    try{
        int type = *(*print->getExpresionToPrint()->getVals()->begin())->getType();

        if(type == 0 || type == 1) {
            if (type == 0) {
                std::cout << execute_formulaInt(print->getExpresionToPrint(), blockNumber) << std::endl << std::endl;
                return true;
            }
            if (type == 1) {
                auto matrix = execute_formulaMatrix(print->getExpresionToPrint(),blockNumber);
                for(auto col = matrix.begin(); col != matrix.end(); col++){
                    std::cout << "| ";
                    for(auto row = (*col).begin(); row != (*col).end(); row++){
                        std::cout << *row << " ";
                    }
                    std::cout << "|" << std::endl;
                }
                std::cout << std::endl;
                return true;
            }
        }else{
            bool flag = true;
            for(auto iter = values.begin(); iter != values.end(); iter++){
                if((*iter)->getName() == *(*print->getExpresionToPrint()->getVals()->begin())->getIdentValue()->getName()){        // jesli nazwa jest zadeklarowana
                    if((*iter)->getRange()->size() == 1 && *((*iter)->getRange()->begin()) == 0){     // jesli jest to zmienna publiczna to na pewno trzeba zrobic przypisanie
                        if((*iter)->getType() == 0 ){
                            std::cout << execute_formulaInt(print->getExpresionToPrint(), blockNumber) << std::endl << std::endl;
                            return true;
                        }else{
                            auto matrix = execute_formulaMatrix(print->getExpresionToPrint(),blockNumber);
                            for(auto col = matrix.begin(); col != matrix.end(); col++){
                                std::cout << "| ";
                                for(auto row = (*col).begin(); row != (*col).end(); row++){
                                    std::cout << *row << " ";
                                }
                                std::cout << "|" << std::endl;
                            }
                            std::cout << std::endl;
                            return true;
                        }
                    }
                    auto num2 = blockNumber.begin();
                    for (auto num = (*iter)->getRange()->begin(); num != (*iter)->getRange()->end(); num++) {
                        if (*num != *num2) {
                            flag = false;
                            break; // to nie ta zmienna
                        }
                        num2++;
                    }
                    if(!flag){
                        flag = true;
                        continue;
                    }
                    if((*iter)->getType() == 0 ){
                        std::cout << execute_formulaInt(print->getExpresionToPrint(), blockNumber) << std::endl << std::endl;
                        return true;
                    }else{
                        auto matrix = execute_formulaMatrix(print->getExpresionToPrint(),blockNumber);
                        for(auto col = matrix.begin(); col != matrix.end(); col++){
                            std::cout << "| ";
                            for(auto row = (*col).begin(); row != (*col).end(); row++){
                                std::cout << *row << " ";
                            }
                            std::cout << "|" << std::endl;
                        }
                        std::cout << std::endl;
                        return true;
                    }
                }
            }
            // jesli przeiterowano cala liste i to znaczy, że zmienna nie istnieje
            std::cout << "Zmienna nie istnieje!" << std::endl << std::flush;
            throw MyException();
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}

bool executor::execute_comparision(Comparision * comparision,std::list<int> blockNumber) {
    try{
        int left = execute_formulaInt(comparision->getLeft(), blockNumber);
        int right = execute_formulaInt(comparision->getRight(), blockNumber);
        if(*comparision->getOper() == 0){
            if(left == right){
                return true;
            }
        }
        if(*comparision->getOper() == 1){
            if(left != right){
                return true;
            }
        }
        if(*comparision->getOper() == 2){
            if(left <=right){
                return true;
            }
        }
        if(*comparision->getOper() == 3){
            if(left < right){
                return true;
            }
        }
        if(*comparision->getOper() == 4){
            if(left >= right){
                return true;
            }
        }
        if(*comparision->getOper() == 5){
            if(left > right){
                return true;
            }
        }
        return false;
    }catch (exception& e){
        
        throw SemanticException();
    }
}

void executor::execute_assigment(AssigmentOrFunctionCall* assigment, std::list<int> blockNumber) {
    try{
        bool flag = true;

        int typeOfConst;
        int type = *(*assigment->getExpression()->getVals()->begin())->getType();

        for(auto iter = values.begin(); iter != values.end(); iter++){
            if((*iter)->getName() == *assigment->getName()){        // jesli nazwa jest zadeklarowana
                if((*iter)->getRange()->size() == 1 && *((*iter)->getRange()->begin()) == 0){     // jesli jest to zmienna publiczna to na pewno trzeba zrobic przypisanie
                    if((*iter)->getType() == 0 ){
                        static_cast<IntVal*>(*iter)->setValue(execute_formulaInt(assigment->getExpression(),blockNumber));
                        return;
                    }else{
                        static_cast<MatrixVal*>(*iter)->setValue(execute_formulaMatrix(assigment->getExpression(),blockNumber));
                        return;
                    }
                }
                auto num2 = blockNumber.begin();
                for (auto num = (*iter)->getRange()->begin(); num != (*iter)->getRange()->end(); num++) {
                    if (*num != *num2) {
                        flag = false;
                        break; // to nie ta zmienna
                    }
                    num2++;
                }
                if(!flag){
                    flag = true;
                    continue;
                }
                if((*iter)->getType() == 0 ){
                    static_cast<IntVal*>(*iter)->setValue(execute_formulaInt(assigment->getExpression(),blockNumber));
                    return;
                }else{
                    static_cast<MatrixVal*>(*iter)->setValue(execute_formulaMatrix(assigment->getExpression(),blockNumber));
                    return;
                }
            }
        }
    // jesli przeiterowano cala liste i to znaczy, że zmienna nie istnieje
        std::cout << "Blad przypisania : zmienna nie istnieje!" << std::endl << std::flush;
        throw MyException();
    }catch (exception& e){
        
        throw SemanticException();
    }
}


Val* executor::execute_declaration(Declaration* declaration, std::list<int> blockNumber) {
    try{
        Val* retval;
        for(auto iter = values.begin(); iter != values.end(); iter++) {
            if ((*iter)->getName() == *declaration->getName()) {  // zmienna o takiej samej nazwie jest już zadeklarowana
                if((*iter)->getRange()->size() == 1 && *((*iter)->getRange()->begin()) == 0) {
                    std::cout << "Deklaracja zmiennej o nazwie juz istniejacej!" << std::endl;
                    throw MyException();
                }
                if ((*iter)->getRange()->size() < declaration->getRange()->size()) {
                    //std::cout << "Deklaracja zmiennej o nazwie juz istniejacej (1)" << std::endl;
                    if (*declaration->getType() == 0) {
                        retval = new IntVal(*declaration->getName(), declaration->getRange(), *declaration->getType(), 0);
                        values.push_back(retval);
                        return retval;
                    } else {
                        std::vector<std::vector<int>> vec;
                        retval = new MatrixVal(*declaration->getName(), declaration->getRange(), *declaration->getType(),
                                               vec);
                        values.push_back(retval);
                        return retval;
                    }
                }
                auto num2 = declaration->getRange()->begin();
                for (auto num = (*iter)->getRange()->begin(); num != (*iter)->getRange()->end(); num++) {
                    if (num != num2) {
                        //std::cout << "Deklaracja zmiennej o nazwie juz istniejacej (2)" << std::endl;
                        if(*declaration->getType() == 1){
                            retval = new IntVal(*declaration->getName(),declaration->getRange(),*declaration->getType(),0);
                            values.push_back(retval);
                            return retval;
                        }else{
                            std::vector<std::vector<int>> vec;
                            retval = new MatrixVal(*declaration->getName(),declaration->getRange(),*declaration->getType(),vec);
                            values.push_back(retval);
                            return retval;
                        }
                    }
                    num2++;
                }
                std::cout << "Deklaracja zmiennej o nazwie juz istniejacej!" << std::endl;
                throw MyException();
            }
        }
        // jesli przeiterowano cala liste i nie znaleziono ani razu zmiennej o takiej nazwie i zasiegu to dodajemy zmienna
        //std::cout << "Deklaracja zmiennej o nowej nazwie" << std::endl;
        if(*declaration->getType() == 0){
            retval = new IntVal(*declaration->getName(),declaration->getRange(),*declaration->getType(),0);
            values.push_back(retval);
            return retval;
        }else{
            std::vector<std::vector<int>> vec;
            retval = new MatrixVal(*declaration->getName(),declaration->getRange(),*declaration->getType(),vec);
            values.push_back(retval);
            return retval;
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}

void executor::execute_whileStatement(WhileStatement* whileStatement, std::list<int> blockNumber ) {
    try{
        while(execute_condition(whileStatement->getCondition(),blockNumber)){
            execute_block(whileStatement->getBlock());
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}

void executor::execute_ifStatement(IfStatement* ifStatement, std::list<int> blockNumber ) {
    try{
        if(execute_condition(ifStatement->getCondition(),blockNumber)){
            execute_block(ifStatement->getBlock());
        }else{
            execute_block(ifStatement->getElseBlock());
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}

bool executor::execute_condition(Condition* condition, std::list<int> blockNumber) {
    try{
        for(auto iter = condition->getComparisionAndOp()->begin(); iter!= condition->getComparisionAndOp()->end();){
            if((*iter).second != nullptr){
                if((*iter).second == 0){
                    return execute_comparision((*iter).first,blockNumber) && execute_comparision((*iter++).first,blockNumber);
                }else{
                    return execute_comparision((*iter).first,blockNumber) || execute_comparision((*iter++).first,blockNumber);
                }
            }else{
                return execute_comparision((*iter).first,blockNumber);
            }
        }
    }catch (exception& e){
        
        throw SemanticException();
    }
}
