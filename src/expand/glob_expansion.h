//
// Created by pennywise on 12/30/25.
//

#ifndef CLASH_GLOB_EXPANSION_H
#define CLASH_GLOB_EXPANSION_H
#include <vector>

#include "../util/model/argvWrapper.h"
#include "../util/model/redirectionWrapper.h"
#include "../util/model/word.h"


class GlobExpansion {
public:
    std::vector<ArgvWrapper> expandArgv(std::vector<ArgvWrapper> argv);
    RedirectionWrapper expandRedirection(RedirectionWrapper redirection);
};


#endif //CLASH_GLOB_EXPANSION_H