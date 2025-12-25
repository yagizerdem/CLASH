#include <iostream>

#include "util/env.h"
#include "util/stringUtil.h"

void setDefaultEnv(char* envp[]) {
    Env* env = Env::getInstance();
    env->loadEnvp(envp);
}

int main(int argc, char* argv[], char* envp[]) {
    setDefaultEnv(envp);

    return 0;
}


