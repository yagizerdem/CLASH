//
// Created by pennywise on 12/14/25.
//

#include "stringUtil.h"
#include <string>


std::string& StringUtil::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
std::string& StringUtil::rtrim(std::string &s)  {
    s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
std::string& StringUtil::trim(std::string &s) {
    return ltrim(rtrim(s));
}

bool StringUtil::isEscapedCharacter(int index, std::string input) {
    int i = index - 1;
    int slashCount = 0;

    while (i >= 0 && input[i] == '\\') {
        slashCount++;
        i--;
    }

    return (slashCount % 2) == 1;
}


std::vector<std::string> StringUtil::split(std::string s, std::string delimiter) {
    if (delimiter.empty()) {
        throw std::invalid_argument("delimiter cannot be empty");
    }

    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}