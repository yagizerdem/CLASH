//
// Created by pennywise on 12/27/25.
//

#ifndef CLASH_WORDSPLITTER_H
#define CLASH_WORDSPLITTER_H
#include <vector>
#include "../util/model/word.h"

class WordSplitter {

public:
    std::vector<Word> wordStream(std::string shellCommand);

private:
    Word::WordContext getWordContext(std::string word);
    std::string normalizeWord(std::string word);
};


#endif //CLASH_WORDSPLITTER_H