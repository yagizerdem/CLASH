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
    cmd1.argv = {"env"};

    Spawn spawn;

    ExecuteProcessResult result =  spawn.executeProcess(cmd1);
    std::cout << result.stdOut << std::endl;
}

TEST(SpawnProcessTest, LongArgumentNoUB) {
    std::string longArg(5000, 'a');

    Command cmd;
    cmd.rawShellCommand = "echo long";
    cmd.argv = {"echo", longArg};

    Spawn spawn;
    ExecuteProcessResult result = spawn.executeProcess(cmd);

    ASSERT_EQ(result.exitCode, 0);
    ASSERT_EQ(result.stdOut, longArg + "\n");
}

TEST(SpawnProcessTest, CommandNotFound) {
    Command cmd;
    cmd.rawShellCommand = "this_command_does_not_exist";
    cmd.argv = {"this_command_does_not_exist"};

    Spawn spawn;
    ExecuteProcessResult result = spawn.executeProcess(cmd);

    ASSERT_NE(result.exitCode, 0);
    ASSERT_TRUE(
        result.stdErr.find("not found") != std::string::npos ||
        result.stdErr.find("No such file") != std::string::npos
    );
}

