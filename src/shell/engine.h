//
// Created by pennywise on 12/29/25.
//

#ifndef CLASH_ENGINE_H
#define CLASH_ENGINE_H
#include <string>

#include "../util/model/command.h"
#include "../util/model/engineResponse.h"
#include "../util/model/pipe.h"
#include "../util/model/executeProcessResult.h"


class Engine {
public:
    EngineResponse handleUserInput(std::string rawUserInput);
private:
    // abstraction  layer over process
    EngineResponse executeCommand(Command command);
    EngineResponse executePipe(Pipe pipe);

    // process
    ExecuteProcessResult executeProcess(Command shellCommand);
    ExecuteProcessResult executePipeProcess(Pipe pipe);

    // built ins
    void exitCLASH(Command command);
    void assignment(Command command);
    void unset(Command command);
    void exportBuiltIn(Command command);
    void cd(Command command);
};


#endif //CLASH_ENGINE_H