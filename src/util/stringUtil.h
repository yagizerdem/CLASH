//
// Created by pennywise on 12/14/25.
//

#ifndef CLASH_STRINGUTIL_H
#define CLASH_STRINGUTIL_H
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>


class StringUtil {
public:
    static std::string &ltrim(std::string &s);
    static std::string &rtrim(std::string &s);
    static std::string &trim(std::string &s);
    static bool isEscapedCharacter(int index, std::string input);
    static std::vector<std::string> split(std::string s, std::string delimiter);
};


#endif //CLASH_STRINGUTIL_H