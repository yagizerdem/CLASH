//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_COMMAND_H
#define CLASH_COMMAND_H
#include <string>

class Command {
public:
    std::string rawShellCommand;

    std::vector<char*> argv;
    std::string redirectStandartInput;
    std::string redirectStandartOutput;
};

#endif //CLASH_COMMAND_H