//
// Created by pennywise on 12/27/25.
//

#ifndef CLASH_WORD_H
#define CLASH_WORD_H
#include <string>

class Word {
public:
    enum WordContext {
        DOUBLE_QUOTE,
        SINGLE_QUOTE,
        BACK_TICK,
        UN_QUOTE,
    };

    std::string lexeme;
    WordContext context;
};

#endif //CLASH_WORD_H