#include <iostream>
#include "util/stringUtil.h"

int main() {

    StringUtil stringutil;
    std::string input = " yagiz erdem ";
    std::cout << input << std::endl;
    stringutil.trim(input);
    std::cout << input << std::endl;




    return 0;
}