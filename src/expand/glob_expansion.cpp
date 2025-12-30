//
// Created by pennywise on 12/30/25.
//

#include "glob_expansion.h"

#include "glob.h"
#include "../util/env.h"

std::vector<Word> GlobExpansion::expand(std::vector<Word> wordStream) {
    std::vector<Word> expanded;
    Env *env = Env::getInstance();
    for (int i =0 ; i < wordStream.size(); i++) {
        Word word = wordStream[i];
        if (word.context != Word::UN_QUOTE) {
            expanded.push_back(word);
        }
        else {



        }
    }

    return  expanded;
}

bool GlobExpansion::shouldGlob(Word w) {


    return  true;
}




