//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_COMMAND_H
#define CLASH_COMMAND_H
#include <string>
#include <vector>

#include "word.h"

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
    std::vector<char*> argv;
    std::string redirectStandartInput; // file name for stdin
    std::string redirectStandartOutput; // file name for stdout
    CommandType commandType;
    // useful properties for assignment
    std::string identifier;
    std::string value;
};

#endif //CLASH_COMMAND_H