//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_SPAWN_H
#define CLASH_SPAWN_H
#include <string>


class Spawn {
public:
    std::string executeProcess(char* argv[]);
    std::string resolveExecutablePath(std::string programName);

};


#endif //CLASH_SPAWN_H