//
// Created by pennywise on 12/29/25.
//


#include "engine.h"

#include <iostream>
#include <ostream>
#include <unistd.h>

#include "../expand/glob_expansion.h"
#include "../expand/ltr_scanner.h"
#include "../expand/tilde.h"
#include "../lexer/commandSplitter.h"
#include "../lexer/wordSplitter.h"
#include "../parser/parseCommand.h"
#include "../process/spawn.h"
#include "../util/env.h"
#include "../util/error/executionError.h"
#include "../util/error/incompleteInput.h"
#include "../util/error/syntaxError.h"

EngineResponse Engine::handleUserInput(std::string rawUserInput) {
    Env *env = Env::getInstance();
    EngineResponse response;
    try {
        rawUserInput = StringUtil::trim(rawUserInput); // normalize
        CommandSplitter command_splitter;
        std::vector<std::variant<Command, Pipe>> commands = command_splitter.commandStream(rawUserInput);

        for (int i = 0 ; i < commands.size(); i++) {
            auto* cmdPtr  = std::get_if<Command>(&commands[i]);
            auto* pipePtr = std::get_if<Pipe>(&commands[i]);
            if (pipePtr != nullptr) {
                Pipe pipe = *pipePtr;
                EngineResponse pipeResponse = executePipe(pipe);

                // std out
                response.stdoutPayload.insert(response.stdoutPayload.end(),
                    pipeResponse.stdoutPayload.begin(), pipeResponse.stdoutPayload.end());

                // std err
                response.stderrPayload.insert(response.stderrPayload.end(),
                    pipeResponse.stderrPayload.begin(), pipeResponse.stderrPayload.end());

                response.interactiveContinue = pipeResponse.interactiveContinue;
                response.lastCommandExitStatus = pipeResponse.lastCommandExitStatus;
                response.success = pipeResponse.success;
                response.terminate = pipeResponse.terminate;

                env->setEnv("?", std::to_string(response.lastCommandExitStatus));
            }
            if (cmdPtr != nullptr) {
                Command shellCommand = *cmdPtr;
                EngineResponse commandResponse = executeCommand(shellCommand);

                // std out
                response.stdoutPayload.insert(response.stdoutPayload.end(),
                    commandResponse.stdoutPayload.begin(), commandResponse.stdoutPayload.end());

                // std err
                response.stderrPayload.insert(response.stderrPayload.end(),
                    commandResponse.stderrPayload.begin(), commandResponse.stderrPayload.end());

                response.interactiveContinue = commandResponse.interactiveContinue;
                response.lastCommandExitStatus = commandResponse.lastCommandExitStatus;
                response.success = commandResponse.success;
                response.terminate = commandResponse.terminate;

                env->setEnv("?", std::to_string(response.lastCommandExitStatus));
            }
        }

        return response;
    }catch (SyntaxError ex) {
        response.stderrPayload.push_back(ex.what());
        response.success = false;
        return response;
    }
    catch (IncompleteInput ex) {
        response.stderrPayload.push_back(ex.what());
        response.success = false;
        response.interactiveContinue = true;
        return response;
    }
    catch (ExecutionError ex) {
        response.stderrPayload.push_back(ex.what());
        response.success = false;
        return response;
    }
    catch (const std::exception& ex) {
        response.stderrPayload.push_back(ex.what());
        response.success = false;
        return response;
    }
    catch (...) {
        response.stderrPayload.push_back("unknown internal error");
        response.success = false;
        return response;
    }
}

EngineResponse Engine::executePipe(Pipe pipe) {
    EngineResponse response;

    // parse commands in  pipe
    for (int i = 0; i < pipe.commands.size(); i++) {
        WordSplitter word_splitter;
        Tilde tildeSubstitution;
        LTR_scanner ltr_scanner;
        ParseCommand parse_command;
        GlobExpansion glob_expansion;

        Command shellCommand = pipe.commands[i];
        shellCommand.rawShellCommand = tildeSubstitution.expandTilde(shellCommand.rawShellCommand);
        shellCommand.rawShellCommand = ltr_scanner.singlePass(shellCommand.rawShellCommand);

        shellCommand.wordStream = word_splitter.wordStream(shellCommand.rawShellCommand);
        shellCommand = parse_command.parse(shellCommand);
        shellCommand.redirectStandartInput = glob_expansion.expandRedirection(shellCommand.redirectStandartInput);
        shellCommand.redirectStandartOutput = glob_expansion.expandRedirection(shellCommand.redirectStandartOutput);
        shellCommand.argv = glob_expansion.expandArgv(shellCommand.argv);

        pipe.commands[i] = shellCommand;
    }

    ExecuteProcessResult result = executePipeProcess(pipe);

    response.stderrPayload.push_back(result.stdErr);
    response.stdoutPayload.push_back(result.stdOut);
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
    GlobExpansion glob_expansion;


    command.rawShellCommand = tildeSubstitution.expandTilde(command.rawShellCommand);
    command.rawShellCommand = ltr_scanner.singlePass(command.rawShellCommand);

    command.wordStream = word_splitter.wordStream(command.rawShellCommand);
    command = parse_command.parse(command);
    command.redirectStandartInput = glob_expansion.expandRedirection(command.redirectStandartInput);
    command.redirectStandartOutput = glob_expansion.expandRedirection(command.redirectStandartOutput);
    command.argv = glob_expansion.expandArgv(command.argv);

    if (command.commandType == Command::EXECUTABLE_COMMAND) {
        ExecuteProcessResult result = executeProcess(command);

        response.stderrPayload.push_back(result.stdErr);
        response.stdoutPayload.push_back(result.stdOut);
        response.success = result.exitCode == 0;
        response.lastCommandExitStatus = result.exitCode;
    }

    if (command.commandType == Command::EXIT) {
        exitCLASH(command);
    }

    if (command.commandType == Command::ASSIGNMENT) {
        assignment(command);
    }

    if (command.commandType == Command::UNSET) {
        unset(command);
    }

    if (command.commandType == Command::EXPORT) {
        exportBuiltIn(command);
    }

    if (command.commandType == Command::CD) {
        cd(command);
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
    std::string exit_status = command.argv[1].lexeme;
    exit(stoi(exit_status));
}

void Engine::assignment(Command command) {
    Env *env = Env::getInstance();
    env->setEnv(command.identifier, command.value);
}

void Engine::unset(Command command) {
    Env *env = Env::getInstance();
    for (int i = 1 ; i < command.argv.size() ; i++) {
        env->unsetEnv(command.argv[i].lexeme);
    }
}

void Engine::exportBuiltIn(Command command) {
    Env *env = Env::getInstance();
    for (int i = 1 ; i < command.argv.size() ; i++) {
        env->exportEnv(command.argv[i].lexeme);
    }
}

void Engine::cd(Command command) {
    Env *env = Env::getInstance();
    std::string path;

    if (command.argv.size() < 2 || command.argv[1].lexeme.empty()) {
        auto home = env->getEnv("HOME");
        if (home.value.empty()) {
            std::cerr << "cd: HOME not set\n";
            return;
        }
        path = home.value;
    }
    else {
        path = command.argv[1].lexeme;
    }

    if (path == ".") {
        return;
    }

    if (chdir(path.c_str()) != 0) {
        throw ExecutionError("cd: no such file or directory");
    }

    // update PWD
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd))) {
        env->setEnv("PWD", cwd);
    }
}
