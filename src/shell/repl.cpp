//
// Created by pennywise on 12/29/25.
//

#include "repl.h"

#include <iostream>
#include <string>
#include <vector>


void REPL::loop() {
    std::string line;
    std::pmr::vector<std::string> args;
    int status;

    do {
        printf("> ");
        line = lsh_read_line();


    } while (status);
}

std::string REPL::lsh_read_line() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}