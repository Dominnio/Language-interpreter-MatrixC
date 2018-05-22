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

    Source* source = new Source("C:\\Users\\Dominik\\CLionProjects\\Language-interpreter-MatrixC\\example.txt");
    Scan scan(*source);
    Parser parser(&scan);
    if(parser.Program()) {
        try {
            executor exec;
            exec.MainExecute(&parser);
            std::cout << "\n========= Wykonanie zakonczone =========";
        }catch (exception& e){
            std::cout << "\n========= Wykonanie zakonczone niepowodzeniem =========\n";
        }
    }else{
        std::cout << "\n========= Parsowanie zakonczone niepowodzeniem =========\n";
    }
    return 0;
}