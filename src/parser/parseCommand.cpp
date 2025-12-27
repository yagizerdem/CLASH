//
// Created by pennywise on 12/27/25.
//

#include "./parseCommand.h"

#include <stdexcept>

#include "../util/stringUtil.h"
#include "../util/model/command.h"

Command ParseCommand::parse(Command shellCommand) {
    Command parsedCommand;
    parsedCommand.rawShellCommand= shellCommand.rawShellCommand;
    parsedCommand.wordStream = shellCommand.wordStream;

    parsedCommand.commandType = classifyCommand(parsedCommand);

    // splitting redirections before handling argv errors !
    parsedCommand.redirectStandartInput = classifyStdInput(parsedCommand);
    parsedCommand.redirectStandartOutput = classifyStdOutput(parsedCommand);
    parsedCommand.argv = classifyArgv(parsedCommand);


    if (parsedCommand.commandType == Command::CD) {
        if (parsedCommand.argv.size() > 2)
            throw std::invalid_argument("cd: too many arguments");
    }

    if (parsedCommand.commandType == Command::UNSET) {
        for (size_t i = 1; i < parsedCommand.argv.size(); ++i) {
            if (!isValidVarName(parsedCommand.argv[i])) {
                throw std::invalid_argument(
                    "unset: `" + parsedCommand.argv[i] +
                    "': not a valid identifier"
                );
            }
        }
    }

    if (parsedCommand.commandType == Command::EXPORT) {
        for (size_t i = 1; i < parsedCommand.argv.size(); ++i) {
            if (!isValidVarName(parsedCommand.argv[i])) {
                throw std::invalid_argument(
                    "export: `" + parsedCommand.argv[i]  +
                    "': not a valid identifier"
                );
            }
        }
    }

    if (parsedCommand.commandType == Command::EXIT) {
        if (parsedCommand.argv.size() > 2) {
            throw std::invalid_argument("exit: too many arguments");
        }
    }

    if (parsedCommand.commandType  == Command::ASSIGNMENT) {
        if (parsedCommand.argv.size() != 1) {
            throw std::logic_error(
                      "internal error: argv must be empty for assignment"
                  );
        }
        int i = 0;
        std::string statement = parsedCommand.argv[i];
        while (i < statement.size()) {
            if (statement[i] == '=') {
                i++; // skip =
                break;
            }
            parsedCommand.identifier += statement[i];
            i++;
        }
        while (i < statement.size()) {
            parsedCommand.value += statement[i];
            i++;
        }
    }

    return  parsedCommand;
}

Command::CommandType ParseCommand::classifyCommand(Command command) {
    if (command.wordStream.empty()) return  Command::EMPTY; // default fallback
    Word firstWord = command.wordStream[0];

    // scan first word to identify command type
    if (firstWord.lexeme == "cd") return  Command::CD;
    if (firstWord.lexeme == "exit") return  Command::EXIT;
    if (firstWord.lexeme == "export") return  Command::EXPORT;
    if (firstWord.lexeme == "unset") return  Command::UNSET;

    if (firstWord.context == Word::UN_QUOTE) {
        for (int i = 0 ; i < firstWord.lexeme.size(); i++) {
            if (firstWord.lexeme[i] == '=') {
                return  Command::ASSIGNMENT;
            }
        }
    }

    return  Command::EXECUTABLE_COMMAND;
}

bool ParseCommand::isValidVarName(const std::string& name) {
    if (name.empty())
        return false;

    if (!std::isalpha(static_cast<unsigned char>(name[0])))
        return false;

    for (size_t i = 1; i < name.size(); ++i) {
        if (!std::isalnum(static_cast<unsigned char>(name[i])))
            return false;
    }

    return true;
}

bool ParseCommand::isBuiltInCommand(Command::CommandType type) {
    return  type == Command::CommandType::UNSET ||
            type == Command::CommandType::CD ||
            type == Command::CommandType::ASSIGNMENT ||
            type == Command::CommandType::EXIT ||
            type == Command::CommandType::EXPORT;
}

std::string ParseCommand::classifyStdInput(Command command) {
    std::string last;

    for (size_t i = 0; i < command.wordStream.size(); ++i) {
        const Word& w = command.wordStream[i];

        if (w.context == Word::UN_QUOTE && w.lexeme == "<") {

            if (i + 1 >= command.wordStream.size()) {
                throw std::invalid_argument(
                    "syntax error near unexpected token `<'"
                );
            }

            const Word& next = command.wordStream[i + 1];

            if (next.lexeme == "<" || next.lexeme == ">" ||
                next.lexeme == "|" || next.lexeme == ";") {
                throw std::invalid_argument(
                    "syntax error near unexpected token `<'"
                );
                }

            last = next.lexeme;
            ++i;
        }
    }

    return last;
}

std::string ParseCommand::classifyStdOutput(Command command) {
    std::string last;

    for (size_t i = 0; i < command.wordStream.size(); ++i) {
        const Word& w = command.wordStream[i];

        if (w.context == Word::UN_QUOTE && w.lexeme == ">") {

            if (i + 1 >= command.wordStream.size()) {
                throw std::invalid_argument(
                    "syntax error near unexpected token `>'"
                );
            }

            const Word& next = command.wordStream[i + 1];

            if (next.lexeme == "<" || next.lexeme == ">" ||
                next.lexeme == "|" || next.lexeme == ";") {
                throw std::invalid_argument(
                    "syntax error near unexpected token `>'"
                );
                }

            last = next.lexeme;
            ++i;
        }
    }

    return last;
}

std::vector<std::string> ParseCommand::classifyArgv(Command command) {
    std::vector<std::string> argv;

    for (size_t i = 0; i < command.wordStream.size(); ++i) {
        const Word& w = command.wordStream[i];

        if (w.context == Word::UN_QUOTE &&
            (w.lexeme == "<" || w.lexeme == ">")) {
            ++i;
            continue;
            }

        argv.push_back(w.lexeme);
    }

    return argv;
}


#include "parseCommand.h"