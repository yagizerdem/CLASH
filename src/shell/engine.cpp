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

void Engine::handleUserInput(std::string rawUserInput) {
    rawUserInput = StringUtil::trim(rawUserInput); // normalize
    CommandSplitter command_splitter;
    std::vector<std::variant<Command, Pipe>> commands = command_splitter.commandStream(rawUserInput);

    for (int i = 0 ; i < commands.size(); i++) {
        auto* cmdPtr  = std::get_if<Command>(&commands[i]);
        auto* pipePtr = std::get_if<Pipe>(&commands[i]);
        if (pipePtr != nullptr) {
            Pipe pipe = *pipePtr;
            executePipe(pipe);
        }
        if (cmdPtr != nullptr) {
            Command shellCommand = *cmdPtr;
            executeCommand(shellCommand);
        }
    }
}

void Engine::executeCommand(Command shellCommand) {

    WordSplitter word_splitter;
    Tilde tildeSubstitution;
    LTR_scanner ltr_scanner;
    ParseCommand parse_command;
    Spawn spawn;

    shellCommand.rawShellCommand = tildeSubstitution.expandTilde(shellCommand.rawShellCommand);
    shellCommand.rawShellCommand = ltr_scanner.singlePass(shellCommand.rawShellCommand);

    shellCommand.wordStream = word_splitter.wordStream(shellCommand.rawShellCommand);
    shellCommand = parse_command.parse(shellCommand);


    ExecuteProcessResult result = spawn.executeProcess(shellCommand);
    result.stdOut = ltr_scanner.normalizeStdOut(result.stdOut);
    result.stdErr = ltr_scanner.normalizeStdOut(result.stdErr);
    ;
}

void Engine::executePipe(Pipe pipe) {
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

    Spawn spawn;
    LTR_scanner ltr_scanner;
    ExecuteProcessResult result = spawn.executePipe(pipe);
    result.stdOut = ltr_scanner.normalizeStdOut(result.stdOut);
    result.stdErr = ltr_scanner.normalizeStdOut(result.stdErr);

    std::cout << result.stdOut << std::endl;
    std::cout << result.stdErr << std::endl;
}
