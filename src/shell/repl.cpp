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

        printf("> ");
        line = lsh_read_line();
        Engine engine;
        EngineResponse response =  engine.handleUserInput(line);

        if (response.success && !response.payload.empty()) {
            std::cout << response.payload << std::endl;
        }

        if (!response.success && !response.errorMessage.empty()) {
            std::cout << response.errorMessage << std::endl;
        }

        env->setEnv("?", std::to_string(response.lastCommandExitStatus));
        flag = response.terminate;

    } while (!flag);
}

std::string REPL::lsh_read_line() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}