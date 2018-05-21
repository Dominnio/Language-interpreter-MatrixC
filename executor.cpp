//
// Created by Dominik on 09.05.2018.
//

#include "executor.h"
#include <algorithm>

void executor::execute_returnStatement(ReturnStatement* returnStatement, std::list<int> blockNumber ) {

}

std::vector<std::vector<int>> executor::execute_formulaMatrix(Expression* expression, std::list<int> blockNumber) {
    /*
    int type = *expression->getLeftVal()->getType();
    if(type == 0){
        std::cout << "Chcesz wykonywac operacje na matrix a masz wartosc int";
    }else{
        std::vector<std::vector<Expression*>*> result;
        std::vector<std::vector<Expression*>*> rightMatrix;

        if(expression->getRightExp() == nullptr){
            result = *expression->getLeftVal()->getMatrixValue();
        }else{
            rightMatrix = execute_expressionHighMatrix(expression->getRightExp(),blockNumber);
            if (*expression->getOper() == 0){
                if((*expression->getLeftVal()->getMatrixValue()->begin())->size() != rightMatrix.size()){
                    std::cout << "Nie można wykonać mnożenia macierzy (niekompatybilna liczba kolumn i wierszy)" << std::endl;
                }else{
                    // wykonanie mnożenia
                }
            }
            if (*expression->getOper() == 1){
                std::cout << "Dzielenie macierzy jest zabronione" << std::endl;

            }
            if (*expression->getOper() == 2){
                if((*expression->getLeftVal()->getMatrixValue()->begin())->size() != (*rightMatrix.begin())->size()){
                    std::cout << "Nie można wykonać dodawania macierzy (niekompatybilna liczba kolumn)";
                }else{
                    if(expression->getLeftVal()->getMatrixValue()->size() != rightMatrix.size()){
                        std::cout << "Nie można wykonać dodawania macierzy (niekompatybilna liczba wierszy)";
                    }else{
                        for(auto iter = expression->getLeftVal()->getMatrixValue()->begin(); iter != expression->getLeftVal()->getMatrixValue()->end(); iter++){
                            result.push_back();
                            for(auto iter2 = (*iter)->begin(); iter2 != (*iter)->end(); iter2++){
                                result.push_back()
                            }
                        }
                        // wykonujemy dodawanie
                    }
                }
            }
            if (*expression->getOper() == 3){
                if((*expression->getLeftVal()->getMatrixValue()->begin())->size() != (*rightMatrix.begin())->size()){
                    std::cout << "Nie można wykonać odejmowania macierzy (niekompatybilna liczba kolumn)";
                }else{
                    if(expression->getLeftVal()->getMatrixValue()->size() != rightMatrix.size()){
                        std::cout << "Nie można wykonać odejmowania macierzy (niekompatybilna liczba wierszy)";
                    }else{
                        // wykonujemy odejmowanie
                    }
                }
            }
        }

    }
     */
}

int executor::execute_formulaInt(Expression* expression, std::list<int> blockNumber) {
    auto opersIter = expression->getOpers()->begin();
    auto valsIter = expression->getVals()->begin();
    std::list<int> valsList;
    std::list<int> operList;
    while(opersIter != expression->getOpers()->end()){
        operList.push_back(*opersIter);
        opersIter++;
    }
    while(valsIter != expression->getVals()->end()){
        valsList.push_back(execute_IntValue(*valsIter,blockNumber));
        valsIter++;
    }
    int result = 0;

    if(operList.size() == 0){
        return *valsList.begin();
    }

    bool isHighOperator = true;
    while(isHighOperator){
        auto valsListIter = valsList.begin();
        auto valsListNextIter = valsList.begin()++;
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
                *valsListIter *= *valsListNextIter;
                valsList.erase(valsListNextIter);
                operList.erase(opersListIter);
                continue;
            }else{
                *valsListIter /= *valsListNextIter;
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
            result += *valsListIter;
        }else{
            result -= *valsListIter;
        }
        valsListIter++;
        opersListIter++;
    }
    return result;
}

int executor::execute_matrixPosition(Ident* ident, std::list<int> blockNumber) {
    bool flag = true;
    for (auto iter = values.begin(); iter != values.end(); iter++) {
        if ((*iter)->getName() == *ident->getName()) {
            if ((*iter)->getRange().size() == 1 && *((*iter)->getRange().begin()) == 0) {
                return static_cast<MatrixVal*>(*iter)->getValue()[execute_IntValue(ident->getPosition()->first, blockNumber)][execute_IntValue(ident->getPosition()->second, blockNumber)];
            }
            if((*iter)->getRange().size() > blockNumber.size()){    // to na pewno nie jest ta zmienna ktorej szukamy, bo ta nie jest widoczna w tym bloku
                continue;
            }
            auto num2 = blockNumber.begin();
            for (auto num = (*iter)->getRange().begin(); num != (*iter)->getRange().end(); num++) {
                if (num != num2) {
                    flag = false;
                    break; // to nie ta zmienna
                }
                num2++;
            }
            if(!flag){
                flag = true;
                continue;
            }
            if((*iter)->getType() == *ident->getType()) {    // jesli typy sie zgadzaja to czas przypisac
                if(*ident->getType() == 0 ){
                    // tu jest blad bo jednak sie typy nie zgadzaja
                }else{
                    return static_cast<MatrixVal*>(*iter)->getValue()[execute_IntValue(ident->getPosition()->first, blockNumber)][execute_IntValue(ident->getPosition()->second, blockNumber)];
                }
            }
        }
    }
}

vector<vector<int>> executor::execute_matrixValue(Value* value, std::list<int> blockNumber) {
    bool flag = true;
    vector<vector<int>> allVal;
    vector<int> rowVal;
    if (*value->getType() == 2) {
        for (auto iter = values.begin(); iter != values.end(); iter++) {
            if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                if ((*iter)->getRange().size() == 1 && *((*iter)->getRange().begin()) == 0) { // zrob przypisanie bo to zmienna publiczna
                    return static_cast<MatrixVal*>(*iter)->getValue();
                }
                if((*iter)->getRange().size() > blockNumber.size()){    // to na pewno nie jest ta zmienna ktorej szukamy, bo ta nie jest widoczna w tym bloku
                    continue;
                }
                auto num2 = blockNumber.begin();
                for (auto num = (*iter)->getRange().begin(); num != (*iter)->getRange().end(); num++) {
                    if (num != num2) {
                        flag = false;
                        break; // to nie ta zmienna
                    }
                    num2++;
                }
                if(!flag){
                    flag = true;
                    continue;
                }
                if((*iter)->getType() == *value->getType()) {    // jesli typy sie zgadzaja to czas przypisac
                    if(*value->getType() == 0 ){
                        // tu jest blad bo jednak sie typy nie zgadzaja
                    }else{
                        return static_cast<MatrixVal*>(*iter)->getValue();
                    }
                }
            }
        }
        std::cout << "Niezadeklarowana zmienna" << std::endl;
        // identyfikator - trzeba siegnac do tablicy w wykonawcy
    }
    if (*value->getType() == 1) {
        auto width = (*value->getMatrixValue()->begin())->size();
        for(auto iter = *value->getMatrixValue()->begin(); iter != *value->getMatrixValue()->end(); iter++){
            if((*iter).size() != width){
                std::cout << "Wiersze macierzy roznia sie liczba elementow";
                return allVal;
            }
        }
        for(auto row = *value->getMatrixValue()->begin(); row != *value->getMatrixValue()->end(); row++){
            rowVal.clear();
            for(auto col = row->begin(); col != row->end(); row++){
                rowVal.push_back(execute_formulaInt(*col,blockNumber));
            }
            allVal.push_back(rowVal);
        }
        return allVal;
    }
    if (*value->getType() == 0) {
        std::cout << "Brak wymaganej wartości matrix" << std::endl;

    }
}

// tutaj trzeba jeszcze rowzazyc jeden przypadek konkretnie to ident ktory jest taki ala[1,2] oraz ala() jako funkcja i to samo dla matrix
int executor::execute_IntValue(Value* value, std::list<int> blockNumber) {
    bool flag = true;
    if (*value->getType() == 2 && value->getIdentValue()->getType() == 0) {
        for (auto iter = values.begin(); iter != values.end(); iter++) {
            if ((*iter)->getName() == *value->getIdentValue()->getName()) {
                if ((*iter)->getRange().size() == 1 && *((*iter)->getRange().begin()) == 0) { // zrob przypisanie bo to zmienna publiczna
                    return static_cast<IntVal*>(*iter)->getValue();
                }
                if((*iter)->getRange().size() > blockNumber.size()){    // to na pewno nie jest ta zmienna ktorej szukamy, bo ta nie jest widoczna w tym bloku
                    continue;
                }
                auto num2 = blockNumber.begin();
                for (auto num = (*iter)->getRange().begin(); num != (*iter)->getRange().end(); num++) {
                    if (num != num2) {
                        flag = false;
                        break; // to nie ta zmienna
                    }
                    num2++;
                }
                if(!flag){
                    flag = true;
                    continue;
                }
                if((*iter)->getType() == *value->getType()) {    // jesli typy sie zgadzaja to czas zwrocic
                    if(*value->getType() == 0 ){
                        return static_cast<IntVal*>(*iter)->getValue();
                    }else{
                        // tu jest blad bo jednak sie typy nie zgadzaja
                    }
                }
            }
        }
        std::cout << "Niezadeklarowana zmienna" << std::endl;
        // identyfikator - trzeba siegnac do tablicy w wykonawcy
    }
    if (*value->getType() == 0) {
        return *value->getIntValue();
    }
    if (*value->getType() == 1) {
        std::cout << "Brak wymaganej wartości int" << std::endl;
    }
}


// tutaj jest pewna nie zerowa szansa, że jest dobrze

void executor::execute_assigmentOrFunctionCall(AssigmentOrFunctionCall* assigment, std::list<int> blockNumber) {
    if(*assigment->getTypeOfConst() == 0){
        execute_assigment(assigment,blockNumber);
    }else{
        execute_functionCall(assigment,blockNumber);
    }
}


void executor::MainExecute(Parser* parser){
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
    FunctionCall* mainCall = new FunctionCall(nullptr);

    execute_functionCall(new AssigmentOrFunctionCall(new string("main"),mainCall),parser->getMainBlock()->getNumber());
}

void executor::execute_functionCall(AssigmentOrFunctionCall* functionCall, std::list<int> blockNumber) {

    Fun* fun = new Fun();
    for(auto iter = functions.begin(); iter != functions.end(); iter++){
        if((*iter)->getName() == *functionCall->getName()){
            fun = *iter;
            break;
        }
    }
    // tutaj dla każdego argumentu trzeba podpisać go do odpowiedniej zmiennej;
    if(functionCall->getFunctionCall()->getArgList() != nullptr) {
        for (auto iter = (fun)->getArguments().begin(); iter != (fun)->getArguments().end(); iter++) {
            auto arg = functionCall->getFunctionCall()->getArgList()->begin();
            if ((*iter)->getType() == 0) {
                static_cast<IntVal *>(*iter)->setValue(execute_formulaInt(*arg, blockNumber));
            } else {
                static_cast<MatrixVal *>(*iter)->setValue(execute_formulaMatrix(*arg, blockNumber));
            }
            arg++;
        }
    }
    execute_block( (fun)->getBlock() );
}

// to jest wykonanie DEKLARACJI FUNKCJI !!!
void executor::execute_function(Function* function, std::list<int> blockNumber) {
    for(auto iter = functions.begin(); iter != functions.end(); iter++) {
        if ((*iter)->getName() == *function->getName()) {       // jeśli funckja o danej nazwie już istnieje
            std::cout << "Redeklaracja funkcji!" << std::endl << std::flush;
        }
    }
    // deklarujemy nowe zmienne, ktore beda widoczne tylko w tej funkcji
    std::list<Val*> arguments;
    for(auto arg = function->getArguments()->begin(); arg != function->getArguments()->end(); arg++) {
        Declaration *declaration = new Declaration((*arg)->second, new std::list<int>, (*arg)->first);
        arguments.push_back(execute_declaration(declaration,function->getBlock()->getNumber()));
    }
    functions.push_back(new Fun(*function->getName(),new Val(),function->getBlock(),arguments));

}


void executor::execute_block(Block* block) {
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
        num++;
    }
}


bool executor::execute_comparision(Comparision * comparision,std::list<int> blockNumber) {
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
        if(left == right){
            return true;
        }
    }
    if(*comparision->getOper() == 4){
        if(left < right){
            return true;
        }
    }
    if(*comparision->getOper() == 5){
        if(left >= right){
            return true;
        }
    }
    if(*comparision->getOper() == 6){
        if(left > right){
            return true;
        }
    }
    return false;
}

void executor::execute_assigment(AssigmentOrFunctionCall* assigment, std::list<int> blockNumber) {
    bool flag = true;
    for(auto iter = values.begin(); iter != values.end(); iter++){
        if((*iter)->getName() == *assigment->getName()){        // jesli nazwa jest zadeklarowana
            if((*iter)->getRange().size() == 1 && *((*iter)->getRange().begin()) == 0){     // jesli jest to zmienna publiczna to na pewno trzeba zrobic przypisanie
                if((*iter)->getType() == *assigment->getTypeOfConst()) {       // jesli typy sie zgadzaja to zrob przypisanie
                    if(*assigment->getTypeOfConst() == 0 ){
                        static_cast<IntVal*>(*iter)->setValue(execute_formulaInt(assigment->getExpression(),blockNumber));
                        return;
                    }else{
                        static_cast<MatrixVal*>(*iter)->setValue(execute_formulaMatrix(assigment->getExpression(),blockNumber));
                        return;
                    }
                }
            }
            if((*iter)->getRange().size() > blockNumber.size()){    // ta zmienna nie ma szans być tą której szukamy, bo nie jest tu widoczna
                continue;
            }
            auto num2 = blockNumber.begin();
            for (auto num = (*iter)->getRange().begin(); num != (*iter)->getRange().end(); num++) {
                if (num != num2) {
                    flag = false;
                    break; // to nie ta zmienna
                }
                num2++;
            }
            if(!flag){
                flag = true;
                continue;
            }
            if((*iter)->getType() == *assigment->getTypeOfConst()) {       // jesli typy sie zgadzaja to zrob przypisanie
                if(*assigment->getTypeOfConst() == 0 ){
                    static_cast<IntVal*>(*iter)->setValue(execute_formulaInt(assigment->getExpression(),blockNumber));
                    return;
                }else{
                    static_cast<MatrixVal*>(*iter)->setValue(execute_formulaMatrix(assigment->getExpression(),blockNumber));
                    return;
                }
            }
        }
    }
    // jesli przeiterowano cala liste i to znaczy, że zmienna nie istnieje
    std::cout << "Zmienna nie istnieje!" << std::endl << std::flush;
    return;
}

Val* executor::execute_declaration(Declaration* declaration, std::list<int> blockNumber) {
    Val* retval;
    std::list<int> range;
    for(auto iter = values.begin(); iter != values.end(); iter++) {
        if ((*iter)->getName() == *declaration->getName()) {  // zmienna o takiej samej nazwie jest już zadeklarowana
            if((*iter)->getRange().size() == 1 && *((*iter)->getRange().begin()) == 0) {
                std::cout << "Redeklaracja zmiennej" << std::endl;
                return nullptr;
            }
            if ((*iter)->getRange().size() < declaration->getRange()->size()) {
                std::cout << "Deklaracja zmiennej o nazwie juz istniejacej (1)" << std::endl;
                if (*declaration->getType() == 1) {
                    retval = new IntVal(*declaration->getName(), *declaration->getRange(), *declaration->getType(), 0);
                    values.push_back(retval);
                    return retval;
                } else {
                    std::vector<std::vector<int>> vec;
                    retval = new MatrixVal(*declaration->getName(), *declaration->getRange(), *declaration->getType(),
                                           vec);
                    values.push_back(retval);
                    return retval;
                }
            }
            auto num2 = *declaration->getRange()->begin();
            for (auto num = (*iter)->getRange().begin(); num != (*iter)->getRange().end(); num++) {
                if (*num != num2) {
                    std::cout << "Deklaracja zmiennej o nazwie juz istniejacej (2)" << std::endl;
                    if(*declaration->getType() == 1){
                        retval = new IntVal(*declaration->getName(),*declaration->getRange(),*declaration->getType(),0);
                        values.push_back(retval);
                        return retval;
                    }else{
                        std::vector<std::vector<int>> vec;
                        retval = new MatrixVal(*declaration->getName(),*declaration->getRange(),*declaration->getType(),vec);
                        values.push_back(retval);
                        return retval;
                    }
                }
                num2++;
            }
            std::cout << "Redeklaracja zmiennej" << std::endl;
            return nullptr;
        }
    }
    // jesli przeiterowano cala liste i nie znaleziono ani razu zmiennej o takiej nazwie i zasiegu to dodajemy zmienna
    std::cout << "Deklaracja zmiennej o nowej nazwie" << std::endl;
    if(*declaration->getType() == 1){
        retval = new IntVal(*declaration->getName(),*declaration->getRange(),*declaration->getType(),0);
        values.push_back(retval);
        return retval;
    }else{
        std::vector<std::vector<int>> vec;
        retval = new MatrixVal(*declaration->getName(),*declaration->getRange(),*declaration->getType(),vec);
        values.push_back(retval);
        return retval;
    }

}

void executor::execute_whileStatement(WhileStatement* whileStatement, std::list<int> blockNumber ) {
    while(execute_condition(whileStatement->getCondition(),blockNumber)){
        execute_block(whileStatement->getBlock());
    }
}

void executor::execute_ifStatement(IfStatement* ifStatement, std::list<int> blockNumber ) {
    if(execute_condition(ifStatement->getCondition(),blockNumber)){
        execute_block(ifStatement->getBlock());
    }else{
        execute_block(ifStatement->getElseBlock());
    }
}

bool executor::execute_condition(Condition* condition, std::list<int> blockNumber) {
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
}
