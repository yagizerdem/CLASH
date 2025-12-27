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
                newWord.lexeme = normalizeWord(currentWord);
                newWord.context = getWordContext(currentWord); // get context by not normalized form !
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
                if (lookAhead == '\\') {
                    if (StringUtil::isEscapedCharacter(i, shellCommand)) currentWord += lookAhead;
                }
                else {
                    currentWord += lookAhead;
                }
            }
        }

    }

    if (!currentWord.empty()) {
        Word newWord;
        newWord.lexeme = normalizeWord(currentWord);
        newWord.context = getWordContext(currentWord);
        result.push_back(newWord);
    }


    return result;
}

Word::WordContext WordSplitter::getWordContext(std::string word) {
    if (word.size() < 2) return  Word::UN_QUOTE;
    if (word[0] ==  '"' || word[word.size() -1] == '"') return Word::DOUBLE_QUOTE;
    if (word[0] ==  '\'' || word[word.size() -1] == '\'') return Word::SINGLE_QUOTE;
    if (word[0] ==  '`' || word[word.size() -1] == '`') return Word::BACK_TICK;

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
