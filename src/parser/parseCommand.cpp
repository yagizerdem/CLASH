//
// Created by pennywise on 12/27/25.
//

#include "./parseCommand.h"
#include "../util/model/command.h"

Command ParseCommand::parse(Command shellCommand) {
    Command parsedCommand;
    parsedCommand.rawShellCommand= shellCommand.rawShellCommand;
    parsedCommand.wordStream = shellCommand.wordStream;



    return  parsedCommand;
}


#include "parseCommand.h"