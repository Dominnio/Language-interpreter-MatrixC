//
// Created by Dominik on 11.04.2018.
//

#include <iostream>
#include "scanner.h"
#include "source.h"
#include "parser.h"
#include "executor.h"


int main()
{
    setbuf(stdout, 0);

    std::cout << "\n========= TEST 1 =========";
    Source* source = new Source("C:\\Users\\Dominik\\CLionProjects\\Language-interpreter-MatrixC\\examples\\example_01.txt");
    Scan* scan = new Scan(*source);
    Parser* parser = new Parser(scan);

    if(parser->Program()) {
        try {
            executor exec;
            exec.MainExecute(parser);
            std::cout << "========= Wykonanie zakonczone =========";
        }catch (exception& e){
            std::cout << "========= Wykonanie zakonczone niepowodzeniem =========\n";
        }
    }else{
        std::cout << "========= Parsowanie zakonczone niepowodzeniem =========\n";
    }


    return 0;
}