//
// Created by pennywise on 12/25/25.
//

#include "spawn.h"
#include <string>
#include "../util/stringUtil.h"
#include<unistd.h>
#include "../util/env.h"
#include <fstream>


std::string Spawn::executeProcess(char* argv[]) {
    StringUtil string_util;
    std::string programName = string_util.convertToCppStyleString(argv[0]);

    pid_t c_pid = fork();
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (c_pid == 0) {
        // CHILD

    }
    else {

    }

}

std::string Spawn::resolveExecutablePath(std::string programName) {
    std::string resolvedPath;
    Env* env = Env::getInstance();
    std::string PATH_variable = env->getEnv("PATH").value;
    if (PATH_variable.empty()) {
        // use default path
        PATH_variable = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
    }
    std::vector<std::string> unix_paths = StringUtil::split(PATH_variable, ":");

    //if program name contains / do not search for path
    if (programName.find('/') != std::string::npos) {
        if (access(programName.c_str(), X_OK) == 0)
            return programName;
        return "";
    }

    for (std::string unix_path : unix_paths) {
        std::string absolutePath = unix_path + "/" + programName;
        bool flag = access(absolutePath.c_str(), X_OK) == 0;
        if (flag) {
            resolvedPath = absolutePath;
            break;
        }
    }

    return  resolvedPath;
}
