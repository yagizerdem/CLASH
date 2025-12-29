
#include "shell/shell.h"
#include "util/env.h"


void setDefaultEnv(char* envp[]) {
    Env* env = Env::getInstance();
    env->loadEnvp(envp);
    std::string HOME = env->getEnv("HOME").value;
    env->setCwd(HOME);
}

int main(int argc, char* argv[], char* envp[]) {
    setDefaultEnv(envp);

    Shell shell;
    shell.start(argc, argv);

    return 0;
}


