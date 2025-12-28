//
// Created by pennywise on 12/16/25.
//

#ifndef CLASH_TILDE_H
#define CLASH_TILDE_H
#include <string>

class Tilde {
public:
    std::string expandTilde(std::string rawCommand);
private:
    std::string getHomeDirByUsername(std::string username);
    std::string getHomeDirCurrentUser();
};


#endif //CLASH_TILDE_H