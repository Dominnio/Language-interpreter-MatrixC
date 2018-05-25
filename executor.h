//
// Created by Dominik on 09.05.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H
#define LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H

#include "parser.h"

class Val;
class Fun;
class Block;

class executor {
public:
    void MainExecute(Parser* parser);
    std::list<Fun*> functions;
    int numberOfActivity = 0;
    Parser* parser;
};
#endif //LANGUAGE_INTERPRETER_MATRIXC_EXECUTOR_H
