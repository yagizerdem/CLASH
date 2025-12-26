//
// Created by pennywise on 12/25/25.
//

#include "spawn.h"
#include <string>
#include "../util/stringUtil.h"
#include<unistd.h>
#include "../util/env.h"
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <array>
#include <fcntl.h>
#include <string.h>
#include <thread>

#include "../util/model/executeProcessResult.h"


ExecuteProcessResult Spawn::executeProcess(Command shellCommand) {
    Env* env = Env::getInstance();
    std::vector<char*> envp;
    int counter = 0;
    for (auto i = env->environment_variables.begin(); i != env->environment_variables.end(); i++) {
        std::string key = i->first;
        std::string value = i->second.value;
        std::string env_val  = key.append("=").append(value);
        envp.push_back(strdup(StringUtil::convertToCString(env_val)));
        counter++;
    }
    envp.push_back(NULL);

    return executeProcess(shellCommand, envp);

}

ExecuteProcessResult Spawn::executeProcess(Command shellCommand, std::vector<char*> envp) {
    const int MSGSIZE = 4096;
    char outbuf[MSGSIZE];
    char errbuf[MSGSIZE];
    int stdOutPipe[2];
    int stdErrPipe[2];
    std::string stdOut = "";
    std::string stdErr = "";

    if (pipe(stdOutPipe) < 0 || pipe(stdErrPipe) < 0)
        exit(EXIT_FAILURE);

    std::string programName = StringUtil::convertToCppStyleString(shellCommand.argv[0]);

    pid_t c_pid = fork();
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (c_pid == 0) {
        // CHILD

        close(stdOutPipe[0]);
        close(stdErrPipe[0]);

        if (!shellCommand.redirectStandartOutput.empty()) {
            int fd = open(
                shellCommand.redirectStandartOutput.c_str(),
                O_WRONLY | O_CREAT | O_TRUNC,
                0644
            );

            if (fd < 0) {
                perror(shellCommand.redirectStandartOutput.c_str());
                _exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else {
            dup2(stdOutPipe[1], STDOUT_FILENO);
            close(stdOutPipe[1]);
        }

        if (!shellCommand.redirectStandartInput.empty()) {
            int fd = open(
                shellCommand.redirectStandartInput.c_str(),
                O_RDONLY
            );

            if (fd < 0) {
                perror(shellCommand.redirectStandartInput.c_str());
                _exit(1);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        dup2(stdErrPipe[1], STDERR_FILENO);
        close(stdErrPipe[1]);

        std::string executablePath = resolveExecutablePath(
            StringUtil::convertToCppStyleString(shellCommand.argv[0]));

        if (executablePath.empty()) {
            std::cerr <<  shellCommand.argv[0] << " executable not found" << std::endl;
            _exit(127);
        }


        execve(StringUtil::convertToCString(executablePath), shellCommand.argv.data(), envp.data());

        perror("execve");
        _exit(126);
    }

    close(stdOutPipe[1]);
    close(stdErrPipe[1]);


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

    std::thread stdOutThread(readStdOut);
    std::thread stdErrThread(readStdErr);

    stdOutThread.join();
    stdErrThread.join();

    int child_exit_status;
    waitpid(c_pid, &child_exit_status, 0);

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

ExecuteProcessResult Spawn::executePipe(Pipe pipeModel,
                                        std::vector<char*> envp)
{
    const int MSGSIZE = 4096;
    int n = pipeModel.commands.size();

    std::vector<std::array<int,2>> outPipes(n);
    std::vector<std::array<int,2>> errPipes(n);

    for (int i = 0; i < n; i++) {
        if (pipe(outPipes[i].data()) < 0) {
            perror("pipe stdout");
            exit(EXIT_FAILURE);
        }
        if (pipe(errPipes[i].data()) < 0) {
            perror("pipe stderr");
            exit(EXIT_FAILURE);
        }
    }

    std::vector<pid_t> pids;

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {

            if (i > 0) {
                dup2(outPipes[i - 1][0], STDIN_FILENO);
            }

            dup2(outPipes[i][1], STDOUT_FILENO);
            dup2(errPipes[i][1], STDERR_FILENO);

            for (int j = 0; j < n; j++) {
                close(outPipes[j][0]);
                close(outPipes[j][1]);
                close(errPipes[j][0]);
                close(errPipes[j][1]);
            }

            std::string exe = resolveExecutablePath(
                StringUtil::convertToCppStyleString(
                    pipeModel.commands[i].argv[0]
                )
            );

            if (exe.empty()) {
                std::cerr << pipeModel.commands[i].argv[0]
                          << ": command not found\n";
                _exit(127);
            }

            execve(
                exe.c_str(),
                pipeModel.commands[i].argv.data(),
                envp.data()
            );

            perror("execve");
            _exit(126);
        }

        pids.push_back(pid);
    }

    for (int i = 0; i < n; i++) {
        close(outPipes[i][1]);
        close(errPipes[i][1]);
    }

    std::unordered_map<int, std::string> stdOuts;
    std::unordered_map<int, std::string> stdErrs;

    auto readPipe = [&](int fd) {
        std::string result;
        char buf[MSGSIZE];
        ssize_t r;
        while ((r = read(fd, buf, MSGSIZE)) > 0) {
            result.append(buf, r);
        }
        return result;
    };

    for (int i = 0; i < n; i++) {
        stdErrs[i] = readPipe(errPipes[i][0]);
        close(errPipes[i][0]);
    }

    std::string finalStdOut = readPipe(outPipes[n - 1][0]);
    close(outPipes[n - 1][0]);

    // wait
    std::unordered_map<int,int> exit_status;
    for (int i = 0; i < pids.size(); i++) {
        int status;
        waitpid(pids[i], &status, 0);
        exit_status[i] = status;
    }

    ExecuteProcessResult result;

    result.stdOut = finalStdOut;

    for (auto &e : stdErrs) {
        if (!e.second.empty()) {
            result.stdErr += e.second;
            result.stdErr += "\n";
        }
    }

    int lastStatus = exit_status[n - 1];

    if (WIFEXITED(lastStatus)) {
        result.exitCode = WEXITSTATUS(lastStatus);
        result.exitedNormally = true;
        result.signaled = false;
    }
    else if (WIFSIGNALED(lastStatus)) {
        result.exitCode = 128 + WTERMSIG(lastStatus);
        result.signaled = true;
        result.signal = WTERMSIG(lastStatus);
        result.exitedNormally = false;
    }

    return result;
}


ExecuteProcessResult Spawn::executePipe(Pipe pipe) {
    Env* env = Env::getInstance();
    std::vector<char*> envp;
    int counter = 0;
    for (auto i = env->environment_variables.begin(); i != env->environment_variables.end(); i++) {
        std::string key = i->first;
        std::string value = i->second.value;
        std::string env_val  = key.append("=").append(value);
        envp.push_back(strdup(StringUtil::convertToCString(env_val)));
        counter++;
    }
    envp.push_back(NULL);

    return executePipe(pipe, envp);
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
