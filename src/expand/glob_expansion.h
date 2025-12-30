//
// Created by pennywise on 12/30/25.
//

#ifndef CLASH_GLOB_EXPANSION_H
#define CLASH_GLOB_EXPANSION_H
#include <vector>

#include "../util/model/word.h"


class GlobExpansion {
public:
    std::vector<Word> expandArgv(std::vector<std::string> argv);
};


#endif //CLASH_GLOB_EXPANSION_H