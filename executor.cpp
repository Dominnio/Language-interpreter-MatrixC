//
// Created by Dominik on 09.05.2018.
//

#include "executor.h"
#include <algorithm>

void executor::MainExecute(Parser* parser){
    for(auto iter = parser->getMainBlock()->getStatementsBlock()->begin(); iter != parser->getMainBlock()->getStatementsBlock()->end(); iter++){
        if((*iter)->getStatementType() == 2)
        {
            execute_declaration(static_cast<Declaration*>(*iter),0);
        }
        if((*iter)->getStatementType() == 6)
        {
            execute_function(static_cast<Function*>(*iter));
        }
    }

    // tutaj na końcu musi być executeFunctonCall( funkcja main )

}

void executor::execute_block(Block* block) {
    for(auto iter = block->getStatementsBlock()->begin(); iter != block->getStatementsBlock()->end(); iter++){
        if((*iter)->getStatementType() == 2)
        {
            execute_declaration(static_cast<Declaration*>(*iter), block->getNumber());
        }
        if((*iter)->getStatementType() == 6)
        {
            execute_function(static_cast<Function*>(*iter));
        }
    }

}

void executor::execute_functionBlock() {

}

void executor::execute_function(Function* function) {
    if(std::find(functionNames.begin(), functionNames.end(),*function->getName()) == functionNames.end())   // jeśli jeszcze nie ma takiej funckji
    {
        functionNames.push_back(*function->getName());
    }else{  // jeśli już jest taka deklaracja
        std::cout << "Redeklaracja funkcji!" << std::endl  <<  std::flush;
    }
}

void executor::execute_functionCall() {

}

void executor::execute_returnStatement() {

}

void executor::execute_assigmentOrFunctionCall() {

}

void executor::execute_functionArguments() {

}
int executor::execute_formulaInt(Expression* expression) {

}

std::vector<std::vector<int>> executor::execute_formulaMatrix(Expression* expression) {

}

void executor::execute_formula(Expression* expression) {
    int type = *expression->getLeftVal()->getType();
    if(type == 0){
        int result;

        if(expression->getRightExp() == nullptr){
            result = *expression->getLeftVal()->getIntValue();
        }else {
            if (*expression->getOper() == 0)
                result = *expression->getLeftVal()->getIntValue() * execute_expressionHighInt(expression->getRightExp());
            if (*expression->getOper() == 1)
                result = *expression->getLeftVal()->getIntValue() / execute_expressionHighInt(expression->getRightExp());
            if (*expression->getOper() == 2)
                result = *expression->getLeftVal()->getIntValue() + execute_expressionHighInt(expression->getRightExp());
            if (*expression->getOper() == 3)
                result = *expression->getLeftVal()->getIntValue() - execute_expressionHighInt(expression->getRightExp());
        }
    }else{
        std::vector<std::vector<Expression*>*> result;
        std::vector<std::vector<Expression*>*> rightMatrix;

        if(expression->getRightExp() == nullptr){
            result = *expression->getLeftVal()->getMatrixValue();
        }else{
            rightMatrix = execute_expressionHighMatrix(expression->getRightExp());
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
}

int executor::execute_expressionHighInt(Expression* expression) {
    int type = *expression->getLeftVal()->getType();
    if(type == 0){
        int result = *expression->getLeftVal()->getIntValue() * execute_expressionHighInt(expression->getRightExp());
        // to będzie mnożenie intów
    }else{
        // to będzie mnoenie macierzy czyli trzeba oddać błąd
    }
}
std::vector<std::vector<Expression*>*> executor::execute_expressionHighMatrix(Expression* expression) {
    int type = *expression->getLeftVal()->getType();
    if(type == 0){
        // to będzie mnożenie intów czyli trzeba oddać błąd bo nie można tak robić
    }else{
        // to będzie mnoenie macierzy
    }
}

void executor::execute_arguments() {

}

void executor::execute_matrixValue() {

}

void executor::execute_matrixRow() {

}

void executor::execute_matrixPosition() {

}

void executor::execute_value() {

}

void executor::execure_ident() {

}

// to jest w 100% dobrze

void executor::execute_assigment(AssigmentOrFunctionCall* assigment, int blockNumber) {
    for(auto iter = values.begin(); iter != values.end(); iter++){
        if((*iter).getName() == *assigment->getName()){        // jesli nazwa jest zadeklarowana
            if((*iter).getRange() == blockNumber || (*iter).getRange() == 0){      // jesli miesci sie w zasiegu
                if((*iter).getType() == *assigment->getTypeOfConst()) {       // jesli typy sie zgadzaja
                    if(*assigment->getTypeOfConst() == 0 ){
                        static_cast<IntVal*>(*iter)->setValue(execute_formulaInt(assigment->getExpression()));
                        return;
                    }else{
                        static_cast<MatrixVal*>(*iter)->setValue(execute_formulaMatrix(assigment->getExpression()));
                        return;
                    }
                }
            }
        }
    }
    // jesli przeiterowano cala liste i to znaczy, że zmienna nie istnieje
    std::cout << "Zmienna nie istnieje!" << std::endl << std::flush;
    return;
}

void executor::execute_declaration(Declaration* declaration, int blockNumber) {
    for(auto iter = values.begin(); iter != values.end(); iter++){
        if((*iter).getName() == *declaration->getName()){  // zmienna o takiej samej nazwie jest już zadeklarowana
            if((*iter).getRange() == *declaration->getRange() || *declaration->getRange() == 0 || (*iter).getRange() == 0 ){     // zmienna o takim samym zasiegu jest już zadeklarowana (szczególny przypadek - zasięg globalny)
                std::cout << "Redeklaracja zmiennej!" << std::endl << std::flush;
                return;
            }else{  // zmienna o takiej nazwie już istanieje ale nie jest w zasięgu
                if(*declaration->getType() == 0){
                    blockNumber = 0;
                }
                if(*declaration->getType() == 0){
                    values.push_back(IntVal(*declaration->getName(),blockNumber,*declaration->getType(),0));
                    return;
                }else{
                    values.push_back(MatrixVal(*declaration->getName(),blockNumber,*declaration->getType(),nullptr));
                    return;
                }
            }
        }
    }
    // jesli przeiterowano cala liste i nie znaleziono ani razu zmiennej o takiej nazwie i zasiegu to dodajemy zmienna
    if(*declaration->getType() == 0){
        blockNumber = 0;
    }
    if(*declaration->getType() == 0){
        values.push_back(IntVal(*declaration->getName(),*declaration->getRange(),*declaration->getType(),0));
        return;
    }else{
        values.push_back(MatrixVal(*declaration->getName(),*declaration->getRange(),*declaration->getType(),nullptr));
        return;
    }
}

void executor::execute_whileStatement(WhileStatement* whileStatement) {
    while(execute_condition(whileStatement->getCondition())){
        execute_block(whileStatement->getBlock());
    }
}

void executor::execute_ifStatement(IfStatement* ifStatement) {
    if(execute_condition(ifStatement->getCondition())){
        execute_block(ifStatement->getBlock());
    }else{
        execute_block(ifStatement->getElseBlock());
    }

}

bool executor::execute_condition(Condition* condition) {
    for(auto iter = condition->getComparisionAndOp()->begin(); iter!= condition->getComparisionAndOp()->end();){
        if((*iter).second != nullptr){
            if((*iter).second == 0){
                return execute_comparision((*iter).first) && execute_comparision((*iter++).first);
            }else{
                return execute_comparision((*iter).first) || execute_comparision((*iter++).first);
            }
        }else{
            return execute_comparision((*iter).first);
        }
    }
}

bool executor::execute_comparision(Comparision * comparision) {
    int* left = comparision->getLeft()->getIntValue();
    int* right = comparision->getRight()->getIntValue();
    if(*comparision->getOper() == 0){
        if(*left == *right){
            return true;
        }
    }
    if(*comparision->getOper() == 1){
        if(*left != *right){
            return true;
        }
    }
    if(*comparision->getOper() == 2){
        if(*left <=*right){
            return true;
        }
    }
    if(*comparision->getOper() == 3){
        if(*left == *right){
            return true;
        }
    }
    if(*comparision->getOper() == 4){
        if(*left < *right){
            return true;
        }
    }
    if(*comparision->getOper() == 5){
        if(*left >= *right){
            return true;
        }
    }
    if(*comparision->getOper() == 6){
        if(*left > *right){
            return true;
        }
    }
    return false;
}
