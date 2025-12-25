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
    Spawn spawn;
    std::string resolvedPath = spawn.resolveExecutablePath("./Desktop/processTest");
    ASSERT_EQ(resolvedPath, "./Desktop/processTest");
}