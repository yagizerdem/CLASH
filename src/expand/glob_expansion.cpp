//
// Created by pennywise on 12/30/25.
//

#include "glob_expansion.h"

#include <cmath>

#include "glob.h"
#include "../util/env.h"
#include "../util/error/executionError.h"

std::vector<ArgvWrapper> GlobExpansion::expandArgv(std::vector<ArgvWrapper> argv) {
    Env *env = Env::getInstance();
    std::string cwd = env->getCwd();
    std::vector<ArgvWrapper> result;

    for (int  i = 0; i < argv.size(); i++) {

        if (argv[i].word.context != Word::UN_QUOTE) {
            result.push_back(argv[i]);
            continue;
        }

        if (argv[i].word.hasEscapedGlobChar || argv[i].word.hasQuotedGlobChar) {
            result.push_back(argv[i]);
            continue;
        }


        Glob glob;
        std::vector<std::string> globResult =  glob.resolveGlob(argv[i].lexeme, cwd);
        if (globResult.empty()) {
            result.push_back(argv[i]);
        }
        else {
            for (std::string expandedPath : globResult) {
                ArgvWrapper new_wrapper;
                new_wrapper.lexeme = expandedPath;
                new_wrapper.word = argv[i].word;
                result.push_back(new_wrapper);
            }
        }
    }

    return result;
}

RedirectionWrapper GlobExpansion::expandRedirection(RedirectionWrapper redirection) {
    if (redirection.lexeme.empty()) return  redirection;

    Env *env = Env::getInstance();
    std::string cwd = env->getCwd();
    Glob glob;

    std::vector<std::string> globResult =  glob.resolveGlob(redirection.lexeme, cwd);

    if (globResult.empty()) {
        return  redirection;
    }

    if (globResult.size() > 1) {
        throw ExecutionError("ambiguous redirect");
    }

    std::string expanded = globResult[0];

    RedirectionWrapper new_wrapper;
    new_wrapper.lexeme = expanded;
    new_wrapper.word = redirection.word;
    return new_wrapper;
}






