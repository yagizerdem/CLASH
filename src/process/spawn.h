//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_SPAWN_H
#define CLASH_SPAWN_H
#include <string>
#include  "../util/model/executeProcessResult.h"
#include "../util/model/pipe.h"

class Spawn {
public:
    // execute single command
    static ExecuteProcessResult executeProcess(std::vector<char*> argv, std::vector<char*> envp);
    static ExecuteProcessResult executeProcess(std::vector<char*> argv);
    // execute  pipe
    static ExecuteProcessResult executePipe(Pipe pipe, std::vector<char*> envp);
    static ExecuteProcessResult executePipe(Pipe pipe);
    // auxilary
    static std::string resolveExecutablePath(std::string programName);
};


#endif //CLASH_SPAWN_H