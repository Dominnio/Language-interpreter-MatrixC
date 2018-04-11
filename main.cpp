//
// Created by Dominik on 11.04.2018.
//

#include <iostream>
#include "scanner.h"
#include "src.h"

int main()
{
    Source* source = new Source("C:\\Users\\Dominik\\CLionProjects\\Language-interpreter-MatrixC\\main.txt");
    Scan scan(*source);

    SymType sym = mainsy;

    while(sym != 37) {
        sym = scan.NextSymbol();
        switch(sym){
            case mainsy: std::cout << "mainsy" << std::endl; break; // 0
            case functionsy: std::cout << "functionsy" << std::endl; break; // 0
            case intsy: std::cout << "intsy" << std::endl; break; // 0
            case matrixsy: std::cout << "matrixsy" << std::endl; break; // 0
            case returnsy: std::cout << "returnsy" << std::endl; break; // 0
            case whilesy: std::cout << "whilesy" << std::endl; break; // 0
            case breaksy: std::cout << "breaksy" << std::endl; break; // 0
            case continuesy: std::cout << "continuesy" << std::endl; break; // 0
            case ifsy: std::cout << "ifsy" << std::endl; break; // 0
            case elsesy: std::cout << "elsesy" << std::endl; break; // 0
            case publicsy: std::cout << "publicsy" << std::endl; break; // 0
            case privatesy: std::cout << "privatesy" << std::endl; break; // 0
            case ident: std::cout << "idensy" << std::endl; break; // 0
            case intconst: std::cout << "intconst" << std::endl; break; // 0
            case times: std::cout << "times" << std::endl; break; // 0
            case divop: std::cout << "divop" << std::endl; break; // 0
            case plusop: std::cout << "plusop" << std::endl; break; // 0
            case minusop: std::cout << "minusop" << std::endl; break; // 0
            case ltop: std::cout << "ltop" << std::endl; break; // 0
            case leop: std::cout << "leop" << std::endl; break; // 0
            case gtop: std::cout << "gtop" << std::endl; break; // 0
            case geop: std::cout << "geop" << std::endl; break; // 0
            case neop: std::cout << "neop" << std::endl; break; // 0
            case eqop: std::cout << "eqop" << std::endl; break; // 0
            case lparent: std::cout << "lparent" << std::endl; break; // 0
            case rparent: std::cout << "rparetn" << std::endl; break; // 0
            case lbracket: std::cout << "lbracket" << std::endl; break; // 0
            case rbracket: std::cout << "rbracket" << std::endl; break; // 0
            case semicolon: std::cout << "semicolon" << std::endl; break; // 0
            case colon: std::cout << "colon" << std::endl; break; // 0
            case becomes: std::cout << "becomes" << std::endl; break; // 0
            case notop: std::cout << "notop" << std::endl; break; // 0
            case andop: std::cout << "andop" << std::endl; break; // 0
            case orop: std::cout << "orop" << std::endl; break; // 0
            case raccessop: std::cout << "raccessop" << std::endl; break; // 0
            case laccessop: std::cout << "laccessop" << std::endl; break; // 0
            case comma: std::cout << "comma" << std::endl; break; // 0
            case others: std::cout << "others" << std::endl; break; // 0
        }
    }
    scan.ScanError(1);
    std::cout << "hello world!";
    return 0;
}