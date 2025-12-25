//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_VARIABLE_H
#define CLASH_VARIABLE_H
#include <string>

class Variable {
public:
    bool isExported;
    std::string value;
    std::string key;
};

#endif //CLASH_VARIABLE_H