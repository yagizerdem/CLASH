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
    // lexer part
    std::string rawShellCommand;
    std::vector<Word> wordStream;

    // parse part + syntax checking
    std::vector<char*> argv;
    std::string redirectStandartInput;
    std::string redirectStandartOutput;
};

#endif //CLASH_COMMAND_H