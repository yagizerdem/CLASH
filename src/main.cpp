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



    return 0;
}


