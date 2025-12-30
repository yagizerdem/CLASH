//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_COMMAND_H
#define CLASH_COMMAND_H
#include <string>
#include <vector>

#include "argvWrapper.h"
#include "redirectionWrapper.h"
#include "word.h"
#include "../stringUtil.h"

class Command {
public:
    enum CommandType {
        EXECUTABLE_COMMAND,
        // builtins
        CD,
        UNSET,
        EXIT,
        EXPORT,
        ASSIGNMENT,
        // fallback
        EMPTY
    };

    // lexer part
    std::string rawShellCommand;
    std::vector<Word> wordStream;

    // parse part + syntax checking
    std::vector<ArgvWrapper> argv;
    RedirectionWrapper redirectStandartInput; // file name for stdin
    RedirectionWrapper redirectStandartOutput; // file name for stdout
    CommandType commandType;
    // useful properties for assignment
    std::string identifier;
    std::string value;

    std::vector<char*> toExecveArgv() {
        std::vector<char*> execveArgv;
        for (int i = 0; i < argv.size(); i++) {
            char* p = const_cast<char*>(argv[i].lexeme.c_str());
            execveArgv.push_back(p);
        }
        execveArgv.push_back(nullptr);
        return  execveArgv;
    }
};

#endif //CLASH_COMMAND_H