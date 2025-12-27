//
// Created by pennywise on 12/28/25.
//

#ifndef CLASH_LTR_SCANNER_H
#define CLASH_LTR_SCANNER_H
#include <string>


class LTR_scanner {
public:
    std::string singlePass(std::string rawShellCommand);
private:
    enum Context {
        SINGLE_QUOTE,
        DOUBLE_QUOTE,
        UNQUOTE,
    };
};


#endif //CLASH_LTR_SCANNER_H