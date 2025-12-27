//
// Created by pennywise on 12/25/25.
//

#include <gtest/gtest.h>
#include "../src/process/spawn.h"
#include "../src/util/env.h"

void setDefaultEnv(char* envp[]) {
    Env* env = Env::getInstance();
    env->loadEnvp(envp);
    std::string HOME = env->getEnv("HOME").value;
    env->setCwd(HOME);
}


int main(int argc, char* argv[], char* envp[]) {
    testing::InitGoogleTest(&argc, argv);
    setDefaultEnv(envp);
    return RUN_ALL_TESTS();
}

TEST(SpawnProcessTest, Basic) {
    std::string resolvedPath = Spawn::resolveExecutablePath("./Desktop/processTest");
    ASSERT_EQ(resolvedPath, "./Desktop/processTest");
}

TEST(SpawnProcessTest, Environ) {
    Command cmd1;
    cmd1.rawShellCommand = "env";
    cmd1.argv = {
        const_cast<char*>("env"),
        nullptr
    };

    Spawn spawn;

    ExecuteProcessResult result =  spawn.executeProcess(cmd1);
    std::cout << result.stdOut << std::endl;
}