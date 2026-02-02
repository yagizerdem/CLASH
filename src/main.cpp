
#include <unistd.h>

#include "shell/shell.h"
#include "util/env.h"

void setDefaultEnv(char* envp[]) {
    Env* env = Env::getInstance();
    env->loadEnvp(envp);
    env->setCwd(env->getCwd());
}

int main(int argc, char* argv[], char* envp[]) {
    setDefaultEnv(envp);

    Shell shell;
    shell.start(argc, argv);

    return 0;
}


