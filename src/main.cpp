#include <iostream>

#include "process/spawn.h"
#include "util/env.h"
#include "util/stringUtil.h"

void setDefaultEnv(char* envp[]) {
    Env* env = Env::getInstance();
    env->loadEnvp(envp);
    std::string HOME = env->getEnv("HOME").value;
    env->setCwd(HOME);
}

int main(int argc, char* argv[], char* envp[]) {
    setDefaultEnv(envp);


    Pipe pipe;

    // ls
    Command cmd1;
    cmd1.rawShellCommand = "ls";
    cmd1.argv = {
        const_cast<char*>("ls"),
        nullptr
    };

    // grep cpp
    Command cmd2;
    cmd2.rawShellCommand = "grep cpp";
    cmd2.argv = {
        const_cast<char*>("grep"),
        const_cast<char*>("cpp"),
        nullptr
    };

    // wc -l
    Command cmd3;
    cmd3.rawShellCommand = "wc -l";
    cmd3.argv = {
        const_cast<char*>("wc"),
        const_cast<char*>("-l"),
        nullptr
    };

    pipe.commands = { cmd1, cmd2, cmd3 };

    ExecuteProcessResult result  = Spawn::executePipe(pipe);

    std::cout << result.stdOut << std::endl;
    std::cout << "hit"<< std::endl;

    return 0;
}


