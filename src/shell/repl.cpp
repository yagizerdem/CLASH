//
// Created by pennywise on 12/29/25.
//

#include "repl.h"

#include <iostream>
#include <string>
#include <vector>

#include "engine.h"
#include "../util/env.h"


void REPL::loop() {
    std::string line;
    std::pmr::vector<std::string> args;
    Env *env = Env::getInstance();
    bool flag = false;



    do {
        std::cout << "CWD : ( "  << env->getCwd() << " )" << std::endl;
         std::cout << "> " << std::endl;

        line = lsh_read_line();
        Engine engine;
        EngineResponse response =  engine.handleUserInput(line);

        for (const auto& line : response.stdoutPayload) {
            if (!line.empty()) std::cout << line << std::endl;
        }

        for (const auto& line : response.stderrPayload) {
            if (!line.empty()) std::cerr << line << std::endl;
        }

        flag = response.terminate;

    } while (!flag);
}

std::string REPL::lsh_read_line() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}