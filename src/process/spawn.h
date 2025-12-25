//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_SPAWN_H
#define CLASH_SPAWN_H
#include <string>
#include  "../util/model/executeProcessResult.h"

class Spawn {
public:
    static ExecuteProcessResult executeProcess(char* argv[], char* envp[]);
    static ExecuteProcessResult executeProcess(char* argv[]);
    static std::string resolveExecutablePath(std::string programName);

};


#endif //CLASH_SPAWN_H