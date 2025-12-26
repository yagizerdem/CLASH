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

    // ls /nonexistent (stderr üretir)
    Command cmd1;
    cmd1.rawShellCommand = "";
    cmd1.argv = {
        const_cast<char*>("cat"),
        nullptr
    };

    // wc -l
    Command cmd2;
    cmd2.rawShellCommand = "wc";
    cmd2.argv = {
        const_cast<char*>("wc"),
        const_cast<char*>("-c"),
        nullptr
    };

    pipe.commands = { cmd1, cmd2 };

    ExecuteProcessResult result  = Spawn::executePipe(pipe);

    std::cout << result.stdOut << std::endl;
    std::cout << result.stdErr << std::endl;
    std::cout << "hit"<< std::endl;

    return 0;
}


