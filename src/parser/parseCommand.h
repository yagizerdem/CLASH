//
// Created by pennywise on 12/27/25.
//

#ifndef CLASH_PARSECOMMAND_H
#define CLASH_PARSECOMMAND_H
#include "../util/model/command.h"
#include <vector>


class ParseCommand {
public:
    Command parse(Command shellCommand);

private:
    Command::CommandType classifyCommand(Command command);
    std::string classifyStdInput(Command command);
    std::string classifyStdOutput(Command command);
    std::vector<char*> classifyArgv(Command command);
    bool isValidVarName(const std::string& name);
    bool isBuiltInCommand(Command::CommandType type);
};


#endif //CLASH_PARSECOMMAND_H