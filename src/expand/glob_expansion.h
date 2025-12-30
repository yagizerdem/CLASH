//
// Created by pennywise on 12/30/25.
//

#ifndef CLASH_GLOB_EXPANSION_H
#define CLASH_GLOB_EXPANSION_H
#include <vector>

#include "../util/model/word.h"


class GlobExpansion {
public:
    std::vector<std::string> expandArgv(std::vector<std::string> argv);
    std::string expandRedirection(std::string redirection);
};


#endif //CLASH_GLOB_EXPANSION_H