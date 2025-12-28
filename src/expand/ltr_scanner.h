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
    std::string singlePass(std::string rawShellCommand);
private:
    enum Context {
        SINGLE_QUOTE,
        DOUBLE_QUOTE,
        UNQUOTE,
    };
    struct CollectWordResult {
        std::string word;
        Context context;
    };
    CollectWordResult collectDoubleQuoteWord(std::string rawCommand,int startIndex);
    CollectWordResult collectSingleQuoteWord(std::string rawCommand,int startIndex);
    CollectWordResult collectUnQuoteWord(std::string rawCommand,int startIndex);

    // expand
    struct CollectVariableResult {
        std::string identifier;
        int consume;
    };
    std::string expand(std::string word, std::unordered_map<std::string, Variable> env);
    CollectVariableResult collectVariable(std::string word, int startIndex);
    std::string collectBackTick(std::string word, int startIndex);
    std::string normalizeVariable(std::string variable);
    std::string normalizeShellCommand(std::string variable);

};


#endif //CLASH_LTR_SCANNER_H