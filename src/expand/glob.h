//
// Created by pennywise on 12/21/25.
//

#ifndef CLASH_GLOB_H
#define CLASH_GLOB_H
#include <atomic>

#include "string"
#include <unordered_set>
#include <vector>
#include <variant>

#include "../util/stringUtil.h"

class Glob {

public:
    bool match(std::string pattern,  std::string wildcard);
    struct Token {
        std::string lexeme;
    };
    struct BracketToken : Token {
        bool negate;
        std::unordered_set<char> allowedCharset;
    };
    std::vector<std::string> resolveGlob(std::string globPattern, std::string cwd);

private:
    std::vector<std::variant<Token, BracketToken>> tokenize(std::string wildcard);
    BracketToken collectBracket(int startIndex, std::string wildcard);
    void classifyBracket(BracketToken &bracketToken);
    std::unordered_set<char> resolveRange(char c1, char c2);
    bool match_recursive(std::string pattern,
        std::vector<std::variant<Glob::Token, Glob::BracketToken>> token_stream,
        int consumed,
        int lookAheadIndex);
    std::string getParentPath(std::string path);
    std::vector<std::string> getAllFoldersUnderPath(std::string path);
    std::vector<std::string> getAllFilesUnderPath(std::string path);
    std::vector<std::string> resolveGlob_recursive(std::string globPattern,
        std::string cwd,
        int segmentIndex,
        std::atomic<bool>& cancellationToken);
    StringUtil stringUtil;
};


#endif //CLASH_GLOB_H