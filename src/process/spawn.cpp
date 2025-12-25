//
// Created by pennywise on 12/25/25.
//

#include "spawn.h"

#include <fcntl.h>
#include <string>
#include "../util/stringUtil.h"
#include<unistd.h>
#include "../util/env.h"
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <string.h>
#include <thread>
#include <signal.h>
#include <termios.h>

#include "../util/model/executeProcessResult.h"


ExecuteProcessResult Spawn::executeProcess(char* argv[]) {
    Env* env = Env::getInstance();
    int size = env->environment_variables.size();
    char* envp[size + 1];
    int counter = 0;
    for (auto i = env->environment_variables.begin(); i != env->environment_variables.end(); i++) {
        std::string key = i->first;
        std::string value = i->second.value;
        std::string env_val  = key.append("=").append(value);
        envp[counter] = strdup(StringUtil::convertToCString(env_val));
        counter++;
    }
    envp[counter] = NULL;

    return executeProcess(argv, envp);

}

ExecuteProcessResult Spawn::executeProcess(char* argv[], char* envp[]) {
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    static struct termios shell_termios;

    tcgetattr(STDIN_FILENO, &shell_termios);


    const int MSGSIZE = 4096;
    char outbuf[MSGSIZE];
    char errbuf[MSGSIZE];
    int stdOutPipe[2];
    int stdErrPipe[2];
    std::string stdOut = "";
    std::string stdErr = "";

    if (pipe(stdOutPipe) < 0 || pipe(stdErrPipe) < 0)
        exit(EXIT_FAILURE);

    StringUtil string_util;
    std::string programName = string_util.convertToCppStyleString(argv[0]);

    pid_t c_pid = fork();
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (c_pid == 0) {
        // CHILD

        // child set own process group
        setpgid(0, 0);

        close(stdOutPipe[0]);
        close(stdErrPipe[0]);

        dup2(stdOutPipe[1], STDOUT_FILENO);
        dup2(stdErrPipe[1], STDERR_FILENO);

        close(stdOutPipe[1]);
        close(stdErrPipe[1]);

        std::string executablePath = resolveExecutablePath(
            StringUtil::convertToCppStyleString(argv[0]));

        if (executablePath.empty()) {
            std::cerr <<  argv[0] << " executable not found" << std::endl;
            _exit(127);
        }


        execve(StringUtil::convertToCString(executablePath), argv, envp);

        perror("execve");
        _exit(126);
    }

    auto readStdOut = [&stdOutPipe, &outbuf, MSGSIZE, &stdOut]( ) -> void {
        size_t n;
        while ((n = read(stdOutPipe[0], outbuf, MSGSIZE)) > 0) {
            stdOut.append(outbuf, n);
        }
        close(stdOutPipe[0]);
    };

    auto readStdErr = [&stdErrPipe, &errbuf, MSGSIZE, &stdErr]( ) -> void {
        size_t n;
        while ((n = read(stdErrPipe[0], errbuf, MSGSIZE)) > 0) {
            stdErr.append(errbuf, n);
        }
        close(stdErrPipe[0]);
    };

    setpgid(c_pid, c_pid);
    tcsetpgrp(STDIN_FILENO, c_pid);
    close(stdOutPipe[1]);
    close(stdErrPipe[1]);

    std::thread stdOutThread(readStdOut);
    std::thread stdErrThread(readStdErr);

    int child_exit_status;
    waitpid(c_pid, &child_exit_status, 0);

    stdOutThread.join();
    stdErrThread.join();

    tcsetpgrp(STDIN_FILENO, getpgrp()); // give termianl contorl to parent again
    tcsetattr(STDIN_FILENO, TCSANOW, &shell_termios);

    // open stdinput again
    int tty = open("/dev/tty", O_RDONLY);
    if (tty >= 0) {
        dup2(tty, STDIN_FILENO);
        close(tty);
    }

    ExecuteProcessResult result;
    result.stdOut = stdOut;
    result.stdErr = stdErr;

    if ( WIFEXITED(child_exit_status) )
    {
        int lastExitStatus = WEXITSTATUS(child_exit_status);

        result.exitCode= lastExitStatus;
        result.signaled = false;
        result.exitedNormally = true;
    }
    else if (WIFSIGNALED(child_exit_status)) {
        int lastExitStatus = 128 + WTERMSIG(child_exit_status);
        int sig = WTERMSIG(child_exit_status);

        result.exitCode = lastExitStatus;
        result.signaled = true;
        result.signal = sig;
        result.exitedNormally = false;

    }
    else {
        int lastExitStatus = EXIT_FAILURE; // default fallback

        result.signaled = false;
        result.exitedNormally = true;
        result.exitCode = lastExitStatus;
    }

    return result;
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
