//
// Created by pennywise on 12/28/25.
//

#ifndef CLASH_LTR_SCANNER_H
#define CLASH_LTR_SCANNER_H
#include <string>
#include <unordered_map>

#include "../util/model/variable.h"


class LTR_scanner {
public:
    std::string singlePass(std::string rawShellCommand, std::unordered_map<std::string, Variable> env);
private:
    enum Context {
        SINGLE_QUOTE,
        DOUBLE_QUOTE,
        UNQUOTE,
    };
};


#endif //CLASH_LTR_SCANNER_H