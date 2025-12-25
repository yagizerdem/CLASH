//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_COMMAND_H
#define CLASH_COMMAND_H
#include <string>

class Command {
public:
    std::string rawShellCommand;

    char* argv[];
};

#endif //CLASH_COMMAND_H