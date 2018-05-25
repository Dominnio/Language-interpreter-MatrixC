//
// Created by Dominik on 23.04.2018.
//

#ifndef LANGUAGE_INTERPRETER_MATRIXC_PARSER_H
#define LANGUAGE_INTERPRETER_MATRIXC_PARSER_H

#include <cstring>
#include <list>
#include <vector>
#include <map>
#include "scanner.h"
#include "util.h"

extern char *AT[];

class Parser
{
    friend class Expression;
public:
    explicit Parser(Scan* scan);
    ~Parser();
    bool Program();
    Block* getMainBlock();
    std::list<Block*> getBlockList();
private:
    Scan* scan;
    Block* mainBlock;
    std::list<Block*> blockList;
    std::list<int> currentRangeList;
    int rangeNumber = 0;
    SymType lastSymbol; // ostatnio pobrany atom ze scanner'a
    enum{ FirstSyntaxError=10, FirstSemanticError=60 };

    void Nexts();   // nastepny symbol
    void accept(SymType atom);
    void SyntaxError(int atom);  // oczekiwany atom
    void SyntaxError1(int atom);  // nieoczekiwany atom

    // FUNKCJE ROZBIORU

    Block* parse_mainBlock ();
    Block* parse_block();
    Block* parse_functionBlock();
    FunctionDeclaration parse_function ();
    FunctionCall parse_functionCall();
    ReturnStatement parse_returnStatement ();
    Expression parse_assigment();
    AssigmentOrFunctionCall* parse_assigmentOrFunctionCall();
    std::list<Expression> parse_functionArguments();
    WhileStatement parse_whileStatement();
    IfStatement parse_ifStatement();
    Condition parse_condition();
    Comparision parse_comparision();
    Expression parse_formula();
    std::list<std::pair<std::string,int>> parse_arguments();
    Ident* parse_ident();
    std::vector<std::vector<Expression>> parse_matrixValue();
    std::vector<Expression> parse_matrixRow();
    std::pair<Expression,Expression> parse_matrixPosition();
    Value* parse_value();
    Declaration parse_declaration();
    int parse_type();
    string parse_identOnlyString();
    int parse_intconst();
    PrintStatement parse_printStatement();

};

#endif //LANGUAGE_INTERPRETER_MATRIXC_PARSER_H