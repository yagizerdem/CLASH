//
// Created by pennywise on 12/29/25.
//


#include "engine.h"

#include <iostream>
#include <ostream>

#include "../expand/ltr_scanner.h"
#include "../expand/tilde.h"
#include "../lexer/commandSplitter.h"
#include "../lexer/wordSplitter.h"
#include "../parser/parseCommand.h"
#include "../process/spawn.h"
#include "../util/env.h"

EngineResponse Engine::handleUserInput(std::string rawUserInput) {
    EngineResponse response;
    rawUserInput = StringUtil::trim(rawUserInput); // normalize
    CommandSplitter command_splitter;
    std::vector<std::variant<Command, Pipe>> commands = command_splitter.commandStream(rawUserInput);

    for (int i = 0 ; i < commands.size(); i++) {
        auto* cmdPtr  = std::get_if<Command>(&commands[i]);
        auto* pipePtr = std::get_if<Pipe>(&commands[i]);
        if (pipePtr != nullptr) {
            Pipe pipe = *pipePtr;
            response = executePipe(pipe);
        }
        if (cmdPtr != nullptr) {
            Command shellCommand = *cmdPtr;
            response = executeCommand(shellCommand);
        }
    }

    return response;
}

EngineResponse Engine::executePipe(Pipe pipe) {
    EngineResponse response;

    // parse commands in  pipe
    for (int i = 0; i < pipe.commands.size(); i++) {
        WordSplitter word_splitter;
        Tilde tildeSubstitution;
        LTR_scanner ltr_scanner;
        ParseCommand parse_command;

        Command shellCommand = pipe.commands[i];
        shellCommand.rawShellCommand = tildeSubstitution.expandTilde(shellCommand.rawShellCommand);
        shellCommand.rawShellCommand = ltr_scanner.singlePass(shellCommand.rawShellCommand);

        shellCommand.wordStream = word_splitter.wordStream(shellCommand.rawShellCommand);
        shellCommand = parse_command.parse(shellCommand);

        pipe.commands[i] = shellCommand;
    }

    ExecuteProcessResult result = executePipeProcess(pipe);

    response.errorMessage = result.stdErr;
    response.payload = result.stdOut;
    response.success = result.exitCode == 0;
    response.lastCommandExitStatus = result.exitCode;

    return  response;
}

EngineResponse Engine::executeCommand(Command command) {
    EngineResponse response;

    WordSplitter word_splitter;
    Tilde tildeSubstitution;
    LTR_scanner ltr_scanner;
    ParseCommand parse_command;


    command.rawShellCommand = tildeSubstitution.expandTilde(command.rawShellCommand);
    command.rawShellCommand = ltr_scanner.singlePass(command.rawShellCommand);

    command.wordStream = word_splitter.wordStream(command.rawShellCommand);
    command = parse_command.parse(command);

    if (command.commandType == Command::EXECUTABLE_COMMAND) {
        ExecuteProcessResult result = executeProcess(command);

        response.errorMessage = result.stdErr;
        response.payload = result.stdOut;
        response.success = result.exitCode == 0;
        response.lastCommandExitStatus = result.exitCode;
    }

    if (command.commandType == Command::EXIT) {
        exitCLASH(command);
    }

    if (command.commandType == Command::ASSIGNMENT) {
        assignment(command);
    }

    return  response;
}

// spawn process
ExecuteProcessResult Engine::executeProcess(Command shellCommand) {
    Spawn spawn;
    LTR_scanner ltr_scanner;

    ExecuteProcessResult result = spawn.executeProcess(shellCommand);
    result.stdOut = ltr_scanner.normalizeStdOut(result.stdOut);
    result.stdErr = ltr_scanner.normalizeStdOut(result.stdErr);

    return  result;
}

ExecuteProcessResult Engine::executePipeProcess(Pipe pipe) {

    Spawn spawn;
    LTR_scanner ltr_scanner;
    ExecuteProcessResult result = spawn.executePipe(pipe);
    result.stdOut = ltr_scanner.normalizeStdOut(result.stdOut);
    result.stdErr = ltr_scanner.normalizeStdOut(result.stdErr);

    return  result;
}



void Engine::exitCLASH(Command command) {
    std::string exit_status = command.argv[1];
    exit(stoi(exit_status));
}

void Engine::assignment(Command command) {
    Env *env = Env::getInstance();
    env->setEnv(command.identifier, command.value);
}
