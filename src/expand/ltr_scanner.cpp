//
// Created by pennywise on 12/28/25.
//

#include "ltr_scanner.h"

#include <unordered_map>

#include "../lexer/commandSplitter.h"
#include "../lexer/wordSplitter.h"
#include "../parser/parseCommand.h"
#include "../process/spawn.h"
#include "../util/stringUtil.h"
#include "../util/model/variable.h"

std::string LTR_scanner::singlePass(std::string rawShellCommand, std::unordered_map<std::string, Variable> env) {
    std::string expanded;
    rawShellCommand = StringUtil::trim(rawShellCommand);
    int lookAheadIndex = 0;

    while (lookAheadIndex <  rawShellCommand.length()) {
        if (rawShellCommand[lookAheadIndex] == ' ') {
            expanded += " ";
            lookAheadIndex++;
            continue;
        }

        CollectWordResult collect_word_result;
        if (rawShellCommand[lookAheadIndex] == '"') {
            collect_word_result = collectSingleQuoteWord(rawShellCommand, lookAheadIndex);
        }
        else if (rawShellCommand[lookAheadIndex] == '\'') {
            collect_word_result = collectDoubleQuoteWord(rawShellCommand, lookAheadIndex);
        }
        else {
            collect_word_result = collectUnQuoteWord(rawShellCommand, lookAheadIndex);
        }

        if (collect_word_result.context == LTR_scanner::SINGLE_QUOTE) {
            expanded += collect_word_result.word;
            lookAheadIndex += collect_word_result.word.length();
        }

        if (collect_word_result.context == LTR_scanner::DOUBLE_QUOTE) {
            std::string expanded_word = expand(collect_word_result.word, env);
            expanded += expanded_word;
            lookAheadIndex += collect_word_result.word.length();
        }

        if (collect_word_result.context == LTR_scanner::UNQUOTE) {
            std::string expanded_word = expand(collect_word_result.word, env);
            expanded += expanded_word;
            lookAheadIndex += collect_word_result.word.length();
        }
    }


    return  expanded;
}




// auxilary
LTR_scanner::CollectWordResult LTR_scanner::collectDoubleQuoteWord(std::string rawShellCommand,int startIndex) {
    CollectWordResult result;
    result.context = LTR_scanner::DOUBLE_QUOTE;
    int curIndex = startIndex;
    result.word += "\"";
    while (!(rawShellCommand[curIndex] == '"' && StringUtil::isEscapedCharacter(curIndex, rawShellCommand))) {
        result.word += rawShellCommand[curIndex];
        curIndex++;
    }
    result.word += "\"";

    return result;
}

LTR_scanner::CollectWordResult LTR_scanner::collectSingleQuoteWord(std::string rawShellCommand,int startIndex) {
    CollectWordResult result;
    result.context = LTR_scanner::SINGLE_QUOTE;
    int curIndex = startIndex;
    result.word += "'";
    while (!(rawShellCommand[curIndex] == '\'' && StringUtil::isEscapedCharacter(curIndex, rawShellCommand))) {
        result.word += rawShellCommand[curIndex];
        curIndex++;
    }
    result.word += "'";

    return result;
}

LTR_scanner::CollectWordResult LTR_scanner::collectUnQuoteWord(std::string rawShellCommand,int startIndex) {
    CollectWordResult result;
    result.context = LTR_scanner::UNQUOTE;
    int curIndex = startIndex;
    while (!(rawShellCommand[curIndex] == ' '||
        rawShellCommand[curIndex] == '\n' ||
        rawShellCommand[curIndex] == '\t')) {
        result.word += rawShellCommand[curIndex];
        curIndex++;
    }

    return result;
}

// expand

std::string LTR_scanner::expand(std::string word, std::unordered_map<std::string, Variable> env) {
    std::string expanded;
    int lookAhead =0;
    while (lookAhead < word.length()) {
        if (word[lookAhead] == '$' && !StringUtil::isEscapedCharacter(lookAhead, word)) {
            CollectVariableResult result = collectVariable(word, lookAhead);
            lookAhead += result.consume;
            std::string normalized = normalizeVariable(result.identifier);
            expanded += env[normalized].value;
        }
        else if (word[lookAhead] == '`' && !StringUtil::isEscapedCharacter(lookAhead, word)) {
            std::string shellCommand = collectBackTick(word, lookAhead);
            std::string normalized = normalizeShellCommand(shellCommand);
            lookAhead += shellCommand.length();

            CommandSplitter command_splitter;
            std::vector<std::variant<Command, Pipe>> commandStream = command_splitter.commandStream(normalized);

            for (std::variant<Command, Pipe> variant : commandStream) {
                auto* cmdPtr  = std::get_if<Command>(&variant);
                auto* pipePtr = std::get_if<Pipe>(&variant);
                WordSplitter word_splitter;
                ParseCommand parse_command;

                if (cmdPtr != nullptr) {
                    Command cmd = *cmdPtr;
                    cmd.wordStream = word_splitter.wordStream(cmd.rawShellCommand);
                    cmd = parse_command.parse(cmd);
                    ExecuteProcessResult result = Spawn::executeProcess(cmd);

                    if (!result.stdOut.empty()) expanded += result.stdOut;
                }
                if (pipePtr != nullptr) {
                    Pipe pipe = *pipePtr;
                    for (int j = 0; j < pipe.commands.size(); j++) {
                        Command cmd = pipe.commands[j];
                        cmd.wordStream = word_splitter.wordStream(cmd.rawShellCommand);
                        cmd = parse_command.parse(cmd);
                        pipe.commands[j]= cmd;
                    }
                    ExecuteProcessResult result = Spawn::executePipe(pipe);

                    if (!result.stdOut.empty()) expanded += result.stdOut;
                }


            }
        }
        else {
            expanded += word[lookAhead];
            lookAhead++;
        }
    }

    return expanded;
}

// auxilary
LTR_scanner::CollectVariableResult LTR_scanner::collectVariable(std::string word,int startIndex) {
    CollectVariableResult result;
    int curIndex = startIndex;
    if (word == "$") {
        result.identifier = "";
        result.consume = 1;
        return result;
    }

    result.identifier.push_back('$');
    curIndex++; // consume $

    if (word[curIndex] == '*' ||
        word[curIndex] == '#' ||
        word[curIndex] == '?') {
        result.consume = 2;
        result.identifier.push_back(word[curIndex]);
        return  result;
    }

    if (word[curIndex]  >= '0' || word[curIndex]  <= '9') {
        result.consume = 2;
        result.identifier.push_back(word[curIndex]);
        return  result;
    }


    if (word[curIndex] == '{') {
        result.identifier.push_back('{');
        curIndex++;
        while (curIndex < word.length() && word[curIndex] != '}') {
            result.identifier.push_back(word[curIndex]);
            curIndex++;
        }

        if (curIndex >= word.length()) {
            throw std::invalid_argument("syntax error: missing '}'");
        }

        result.identifier.push_back('}');
        result.consume =  result.identifier.length();
        return result;
    }


    while (startIndex < word.length() &&  word[curIndex] != ' ') {
        result.identifier.push_back(word[curIndex]);
        curIndex++;
    }
    result.consume = result.identifier.length();
    return result;

}

std::string LTR_scanner::collectBackTick(std::string word, int startIndex) {
    std::string nestedShellCommand;
    int curIndex = startIndex;
    nestedShellCommand.push_back('`');
    curIndex++; // consume `

    while (curIndex < word.length() && word[curIndex] != '`') {
        nestedShellCommand.push_back(word[curIndex]);
        curIndex++;
    }

    if (curIndex >= word.length()) {
        throw std::invalid_argument("syntax error: missing '`'");
    }

    nestedShellCommand.push_back('`');
    return nestedShellCommand;
}

std::string LTR_scanner::normalizeShellCommand(std::string cmd) {
    return  cmd.substr(1, cmd.length() - 2);
}

std::string LTR_scanner::normalizeVariable(std::string variable) {
    std::string normalizedVariable;
    int lookAhead = 0;
    if (variable[lookAhead] == '$') {
        lookAhead++; // skip $
    }

    if (variable[lookAhead] == '{') {
        lookAhead++;
        while (variable[lookAhead] != '}') {
            normalizedVariable += variable[lookAhead];
            lookAhead++;
        }
        if (lookAhead >= variable.length()) {
            throw std::invalid_argument("syntax error: missing '}'");
        }
        return normalizedVariable;
    }

    while (lookAhead < variable.length() &&  variable[lookAhead] != ' ') {
        normalizedVariable += variable[lookAhead];
        lookAhead++;
    }

    if (lookAhead >= variable.length()) {
        throw std::invalid_argument("syntax error: missing '}'");
    }

    return normalizedVariable;
}
