//
// Created by pennywise on 12/26/25.
//

#include  "commandSplitter.h"

#include "../util/stringUtil.h"
#include "../util/error/syntaxError.h"

char CommandSplitter::peek() {
    if (lookAheadIndex >= rawShellCommand.size()) {
        return EOF;
    }
    return  rawShellCommand[lookAheadIndex];
}

char CommandSplitter::move() {
    if (lookAheadIndex >= rawShellCommand.size()) {
        return EOF;
    }
    char c =  rawShellCommand[lookAheadIndex];
    lookAheadIndex++;
    return c;
}


std::vector<std::variant<Command, Pipe> > CommandSplitter::commandStream(std::string rawShellCommand) {
    this->lookAheadIndex = 0;
    this->rawShellCommand = StringUtil::trim(rawShellCommand);

    std::vector<std::variant<Command, Pipe>> commandStream;
    std::vector<CommandSegment> command_segments ;

    while (peek() != EOF) {
        std::string rawShellCommand = processCommand();
        rawShellCommand = StringUtil::trim(rawShellCommand);

        char prevSplitter  = ' ';
        // consume splitters
        while (peek() == '\n' ||
            peek() == ';' ||
            peek() == '|' ) {

            char currentSplitter = peek();
            if (prevSplitter !=  ' ') {
                if (getSplitterPresedence(prevSplitter) == getSplitterPresedence(currentSplitter) &&
                    (currentSplitter == '|' || prevSplitter == '|')) {
                    throw SyntaxError(std::string("syntax error: unexpected consecutive operator `") +
                        prevSplitter + currentSplitter + "`");
                }
            }

            if (getSplitterPresedence(currentSplitter) >= getSplitterPresedence(prevSplitter)) {
                prevSplitter = currentSplitter;
            }
            move(); // consume splitter
        }


        if (!rawShellCommand.empty()) {
            CommandSegment commandSegment;
            commandSegment.token = rawShellCommand;
            commandSegment.isCommand = true;
            commandSegment.isSplitter = false;
            command_segments.push_back(commandSegment);
        }
        if (prevSplitter != ' ') {
            CommandSegment commandSegment;
            commandSegment.token = prevSplitter;
            commandSegment.isCommand = false;
            commandSegment.isSplitter = true;

            if (!command_segments.empty() &&
                command_segments.back().isSplitter) {
                char prev_splitter = command_segments.back().token[0];

                if (getSplitterPresedence(prev_splitter) < getSplitterPresedence(commandSegment.token[0])) {
                    command_segments.pop_back();
                    command_segments.push_back(commandSegment);
                }

            }
            else {
                command_segments.push_back(commandSegment);
            }

        }
    }

    std::vector<Command> queue;
    for (int i = 0; i < command_segments.size(); i++) {
        if (command_segments[i].isSplitter) continue;
        bool flag = false; // indicates shell command is pipe item
        bool breakPipe = false; // if true shell command is last item in pipe

        if (i > 0 && (command_segments[i - 1].isSplitter && command_segments[i - 1].token == "|")) {
            flag = true;
        }
        if (i + 1 < command_segments.size() && (command_segments[i + 1].isSplitter && command_segments[i + 1].token == "|")) {
            flag = true;
        }

        if (!command_segments[i + 1].isSplitter || (command_segments[i + 1].token != "|"))
            breakPipe = true;


        if (flag == true) {
            Command shellCommand;
            shellCommand.rawShellCommand = command_segments[i].token;
            queue.push_back(shellCommand);

            if (breakPipe) {
                // flush
                Pipe pipe;
                for (auto cmd : queue) pipe.commands.push_back(cmd);
                commandStream.push_back(pipe);
                queue.clear();
            }
        }
        else {
            if (!queue.empty()) {
                Pipe pipe;
                for (auto cmd : queue) pipe.commands.push_back(cmd);
                commandStream.push_back(pipe);
                queue.clear();
            }
            Command shellCommand;
            shellCommand.rawShellCommand = command_segments[i].token;
            commandStream.push_back(shellCommand);
        }


    }

    if (!queue.empty()) {
        Pipe pipe;
        for (auto cmd : queue) pipe.commands.push_back(cmd);
        commandStream.push_back(pipe);
    }

    return commandStream;
}

std::string CommandSplitter::processCommand() {
    std::string rawShellCommand;
    while (peek() != EOF) {
        if ((peek() == '\n' || peek() == '|' || peek() == ';') && !StringUtil::isEscapedCharacter(lookAheadIndex, this->rawShellCommand)) {
            return  rawShellCommand;
        }

        if (peek() == '\"' && !StringUtil::isEscapedCharacter(lookAheadIndex,  rawShellCommand)) {
            std::string doubleQuotedWord = processDoubleQuote();
            rawShellCommand += doubleQuotedWord;
        }
        else if (peek() == '\'' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            std::string singleQuotedWord = processSingleQuote();
            rawShellCommand += singleQuotedWord;
        }
        else if (peek() == '`' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            std::string backTickWord = processBackTick();
            rawShellCommand += backTickWord;
        }
        else {
            std::string unquoted = processUnquoted();
            rawShellCommand += unquoted;

            if (peek() == ' ') {
                rawShellCommand += " ";
                move(); // consume space
            }
        }
    }

    return  rawShellCommand;
}

std::string CommandSplitter::processDoubleQuote() {
    std::string doubleQuotedWord;
    doubleQuotedWord += peek(); // "
    move(); // consume "

    while (peek() != EOF) {
        if (peek() == '\"' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            doubleQuotedWord += peek();
            move();
            return  doubleQuotedWord;
        }
        else if (peek() == '`' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            std::string backTickWord = processBackTick();
            doubleQuotedWord += backTickWord;
        }
        else {
            doubleQuotedWord += peek();
            move(); // consume char
        }
    }

    throw SyntaxError("syntax error: unterminated double quote (\") at position " +
        std::to_string(lookAheadIndex));

}

std::string CommandSplitter::processSingleQuote() {
    std::string singleQuotedWord;
    singleQuotedWord += peek(); // '
    move(); // consume '

    while (peek() != EOF) {
        if (peek() == '\'' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            singleQuotedWord += peek();
            move();
            return  singleQuotedWord;
        }
        singleQuotedWord += peek();
        move(); // consume char
    }

    throw SyntaxError("syntax error: unterminated single quote (\') at position " +
    std::to_string(lookAheadIndex));

}

std::string CommandSplitter::processBackTick() {
    std::string backTickWord;
    backTickWord += peek(); // `
    move(); // consume `

    while (peek() != EOF) {
        if (peek() == '`' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            backTickWord += peek();
            move();
            return  backTickWord;
        }
        backTickWord += peek();
        move(); // consume char
    }

    throw SyntaxError("syntax error: unterminated single quote (`) at position " +
    std::to_string(lookAheadIndex));

}

std::string CommandSplitter::processUnquoted() {
    std::string unquotedWord;
    while (peek() != EOF) {
        if ((peek() == '\n' || peek() == '|' || peek() == ';' || peek() == ' ') && !StringUtil::isEscapedCharacter(lookAheadIndex, this->rawShellCommand)) {
            break;
        }

        if (peek() == '`' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            std::string backTickWord = processBackTick();
            unquotedWord += backTickWord;
        }
        else if (peek() == '\'' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            std::string singleQuotedWord = processSingleQuote();
            unquotedWord += singleQuotedWord;
        }
        else if (peek() == '\"' && !StringUtil::isEscapedCharacter(lookAheadIndex, rawShellCommand)) {
            std::string doubleQuotedWord = processDoubleQuote();
            unquotedWord += doubleQuotedWord;
        }
        else {
            unquotedWord += peek();
            move();
        }
    }

    return  unquotedWord;
}

int  CommandSplitter::getSplitterPresedence(char splitter) {
    if (splitter == ';' || splitter == '|') return 2;
    if (splitter == '\n' ) return 1;
    return  0;
}

#include "commandSplitter.h"