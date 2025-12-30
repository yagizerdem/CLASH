//
// Created by pennywise on 12/27/25.
//

#include "wordSplitter.h"

#include "../util/stringUtil.h"

std::vector<Word> WordSplitter::wordStream(std::string shellCommand) {
    Word::WordContext context = Word::UN_QUOTE; // usually executable name dont start with quotes
    std::vector<Word> result;

    std::string currentWord;
    for (int i = 0 ; i < shellCommand.length(); i++) {
        char lookAhead = shellCommand.at(i);
        // handle context of current word
        if (lookAhead == '"' &&
            !StringUtil::isEscapedCharacter(i, shellCommand) &&
            (context == Word::DOUBLE_QUOTE || context == Word::UN_QUOTE)) {

            context = context == Word::UN_QUOTE ? Word::DOUBLE_QUOTE : Word::UN_QUOTE ;
        }
        else if (lookAhead == '\'' &&
            !StringUtil::isEscapedCharacter(i, shellCommand)&&
            (context == Word::SINGLE_QUOTE || context == Word::UN_QUOTE)) {
            context = context == Word::UN_QUOTE ? Word::SINGLE_QUOTE : Word::UN_QUOTE ;
            }
        else if (lookAhead == '`' &&
            !StringUtil::isEscapedCharacter(i, shellCommand) && (
                context == Word::BACK_TICK || context == Word::UN_QUOTE
            )) {
            context = context == Word::UN_QUOTE ? Word::BACK_TICK : Word::UN_QUOTE ;
            }

        if (context != Word::UN_QUOTE) {
            currentWord += lookAhead;
        }
        else {
            if (lookAhead == ' ' || lookAhead == '\t' || lookAhead == '\n' || lookAhead == '\r') {
                // break word
                Word newWord;
                newWord.lexeme = normalizeBackSlash(normalizeWord(currentWord));
                newWord.context = getWordContext(currentWord); // get context by not normalized form !


                if (newWord.context == Word::UN_QUOTE) {
                    newWord.hasEscapedGlobChar = hasEscapedGlobChar(currentWord);
                    newWord.hasQuotedGlobChar = hasQuotedGlobChar(currentWord);
                }
                else {
                    newWord.hasEscapedGlobChar = hasEscapedGlobChar(currentWord);
                    newWord.hasQuotedGlobChar = true;
                }

                if (!newWord.lexeme.empty()) {
                    result.push_back(newWord);
                }
                else {
                    if (newWord.context != Word::UN_QUOTE) {
                        newWord.context = getWordContext(currentWord); // get context by not normalized form !
                        result.push_back(newWord);
                    }
                }

                currentWord = "";

            }
            else {
                currentWord += lookAhead;
            }
        }

    }

    if (!currentWord.empty()) {
        Word newWord;
        newWord.lexeme = normalizeBackSlash(normalizeWord(currentWord));
        newWord.context = getWordContext(currentWord);
        newWord.hasEscapedGlobChar = hasEscapedGlobChar(currentWord);
        newWord.hasQuotedGlobChar = hasQuotedGlobChar(currentWord);

        result.push_back(newWord);
    }


    return result;
}

Word::WordContext WordSplitter::getWordContext(std::string word) {
    if (word.size() < 2) return  Word::UN_QUOTE;
    if (word[0] ==  '"' && word[word.size() -1] == '"') return Word::DOUBLE_QUOTE;
    if (word[0] ==  '\'' && word[word.size() -1] == '\'') return Word::SINGLE_QUOTE;
    if (word[0] ==  '`' && word[word.size() -1] == '`') return Word::BACK_TICK;

    return  Word::UN_QUOTE;
}

std::string WordSplitter::normalizeWord(std::string word) {
    if (word.empty()) return word;
    if (word[0] == '"' && word[word.size() -1] == '"' ||
        word[0] == '\'' && word[word.size() -1] == '\'' ||
        word[0] == '`' && word[word.size() -1] == '`') {
        return word.substr(1, word.size() - 2);
    };

    return word;  // unquoted or fallback
}

std::string WordSplitter::normalizeBackSlash(std::string word) {
    std::string normalized;

    for (int i = 0; i < word.size(); i++) {
        if (word[i] == '\\' && StringUtil::isEscapedCharacter(i, word)) {
            normalized += word[i];
        }
        else {
            if (word[i] != '\\') normalized += word[i];
        }

    }

    return  normalized;
}

bool WordSplitter::hasEscapedGlobChar(std::string rawWord) {
    for (int i = 0; i < rawWord.length(); i++) {
        if ((rawWord[i] == '*' || rawWord[i] == '?' || rawWord[i] == '[')
            && StringUtil::isEscapedCharacter(i, rawWord)) {
            return true;
        }
    }
    return false;
}

bool WordSplitter::hasQuotedGlobChar(const std::string rawWord) {
    Word::WordContext ctx = Word::UN_QUOTE;

    for (int i = 0; i < rawWord.length(); i++) {
        char c = rawWord[i];

        if (c == '\'' && !StringUtil::isEscapedCharacter(i, rawWord)) {
            if (ctx == Word::UN_QUOTE) ctx = Word::SINGLE_QUOTE;
            else if (ctx == Word::SINGLE_QUOTE) ctx = Word::UN_QUOTE;
            continue;
        }

        if (c == '"' && !StringUtil::isEscapedCharacter(i, rawWord)) {
            if (ctx == Word::UN_QUOTE) ctx = Word::DOUBLE_QUOTE;
            else if (ctx == Word::DOUBLE_QUOTE) ctx = Word::UN_QUOTE;
            continue;
        }

        if (ctx != Word::UN_QUOTE &&
            (c == '*' || c == '?' || c == '[')) {
            return true;
            }
    }

    return false;
}