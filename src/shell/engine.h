//
// Created by pennywise on 12/29/25.
//

#ifndef CLASH_ENGINE_H
#define CLASH_ENGINE_H
#include <string>

#include "../util/model/command.h"
#include "../util/model/pipe.h"


class Engine {
public:
    void handleUserInput(std::string rawUserInput);
private:
    void executeCommand(Command shellCommand);
    void executePipe(Pipe pipe);
};


#endif //CLASH_ENGINE_H