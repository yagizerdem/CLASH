//
// Created by pennywise on 12/29/25.
//

#include "shell.h"

#include <iostream>
#include <string>

#include "engine.h"
#include "repl.h"
#include "../util/env.h"
#include "../util/stringUtil.h"
#include "../util/model/executeProcessResult.h"
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

void Shell::start(int argc, char *argv[]) {
    Env* env = Env::getInstance();

    // default for every initialization type (from file | interactive reading | from argv)
    std::string programName = argv[0];
    env->setEnv("0", programName);

    if (argc == 1) {
        runInteractiveMode();
    }
    else if (argc >= 2 && StringUtil::convertToCppStyleString(argv[1]) == "-c") {
        runArgvMode(argc, argv);
    }
    else if (argc >= 2) {
        runFileMode(argc, argv);
    }
}


void Shell::runInteractiveMode() {
    Env* env = Env::getInstance();
    env->setEnv("?", "0");
    env->setEnv("*", "");
    env->setEnv("#", "0");

    // check std input source is terminal or redirected from other source
    int std_in_fd;
    bool isTerminal = isatty(STDIN_FILENO) == 1;

    if (isTerminal) {
        REPL repl;
        repl.loop();
    }
    else {
        std::ostringstream ss;
        ss << std::cin.rdbuf();

        std::string userInput = ss.str();

        Engine engine;
        EngineResponse response =   engine.handleUserInput(userInput);

        for (const auto& line : response.stdoutPayload) {
            std::cout << line << '\n';
        }

        for (const auto& line : response.stderrPayload) {
            std::cerr << line << '\n';
        }
    }

}

void Shell::runArgvMode(int argc, char *argv[]) {
    Env* env = Env::getInstance();
    // collect arguments
    if (argc == 2) {
        env->setEnv("0", argv[0]); // file name
        env->setEnv("1", argv[1]); // -c
        env->setEnv("?", "0");
        env->setEnv("#", "1");
        env->setEnv("*", StringUtil::convertToCppStyleString(argv[1]));

        std::string fileName = argv[0];

        // execute shell commands inside file
        bool flag = isRegularFile(fileName);
        if (flag) {
            std ::string rawUserInput = readAllText(fileName);
            rawUserInput = StringUtil::trim(rawUserInput); // normalize
            Engine engine;
            EngineResponse response =  engine.handleUserInput(rawUserInput);

            for (const auto& line : response.stdoutPayload) {
                std::cout << line << '\n';
            }

            for (const auto& line : response.stderrPayload) {
                std::cerr << line << '\n';
            }
        }
    }
    else if (argc >= 3 ) {
        std::string shellCommand = argv[2];
        env->setEnv("?", "0");
        std::string argvAccumulator;
        for (int i = 3 ; i < argc ; i++) {
            env->setEnv(std::to_string(i-2), argv[i]);
            argvAccumulator.append(argv[i]);
            argvAccumulator.append(" ");
        }
        argvAccumulator = argvAccumulator.substr(0,argvAccumulator.size() -1);
        env->setEnv("#", std::to_string(argc - 3));
        env->setEnv("*", argvAccumulator);

        // execute raw shell command
        Engine engine;
        EngineResponse response =   engine.handleUserInput(shellCommand);

        for (const auto& line : response.stdoutPayload) {
            std::cout << line << '\n';
        }

        for (const auto& line : response.stderrPayload) {
            std::cerr << line << '\n';
        }
    }

}

void Shell::runFileMode(int argc, char *argv[]) {
    std::string fileName = argv[1];

    Env* env = Env::getInstance();
    env->setEnv("?", "0");
    env->setEnv("#", std::to_string(argc - 2));
    std::string argvAccumulator;
    for (int i = 2 ; i < argc ; i++) {
        argvAccumulator.append(argv[i]);
        argvAccumulator.append(" ");
        env->setEnv(std::to_string(i - 1), argv[i]);
    }
    argvAccumulator = argvAccumulator.substr(0,argvAccumulator.size() -1);
    env->setEnv("*", argvAccumulator);

    bool flag = isRegularFile(fileName);
    if (flag) {
        std ::string rawUserInput = readAllText(fileName);
        rawUserInput = StringUtil::trim(rawUserInput); // normalize
        Engine engine;
        EngineResponse response = engine.handleUserInput(rawUserInput);

        for (const auto& line : response.stdoutPayload) {
            std::cout << line << '\n';
        }

        for (const auto& line : response.stderrPayload) {
            std::cerr << line << '\n';
        }

    }
}


bool Shell::isRegularFile(const std::string path) {
    struct stat sb;
    if (stat(path.c_str(), &sb) != 0) {
        return false; // file not exist or permission denied
    }
    return S_ISREG(sb.st_mode);
}

std::string Shell::readAllText(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("cannot open file: " + filename);
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}