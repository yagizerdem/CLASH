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
#include <string.h>
#include <thread>

#include "../util/model/executeProcessResult.h"


ExecuteProcessResult Spawn::executeProcess(std::vector<char*> argv) {
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

    return executeProcess(argv, envp);

}

ExecuteProcessResult Spawn::executeProcess(std::vector<char*> argv, std::vector<char*> envp) {
    const int MSGSIZE = 4096;
    char outbuf[MSGSIZE];
    char errbuf[MSGSIZE];
    int stdOutPipe[2];
    int stdErrPipe[2];
    std::string stdOut = "";
    std::string stdErr = "";

    if (pipe(stdOutPipe) < 0 || pipe(stdErrPipe) < 0)
        exit(EXIT_FAILURE);

    std::string programName = StringUtil::convertToCppStyleString(argv[0]);

    pid_t c_pid = fork();
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (c_pid == 0) {
        // CHILD

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


        execve(StringUtil::convertToCString(executablePath), argv.data(), envp.data());

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

ExecuteProcessResult Spawn::executePipe(Pipe pipeModel, std::vector<char*> envp) {
    const int MSGSIZE = 4096;
    std::vector<std::array<int, 2>> std_out_channels;
    std::vector<std::array<int, 2>> std_err_channels;
    std::unordered_map<int, std::string> std_outs;
    std::unordered_map<int, std::string> std_errs;
    std::unordered_map<int, int> exit_status;
    std::vector<int> pids;

    for (int i = 0; i < pipeModel.commands.size(); i++) {
        std::array<int, 2> p_out;
        std::array<int, 2> p_err;
        if (pipe(p_out.data()) < 0) {
            exit(EXIT_FAILURE);
        }
        if (pipe(p_err.data()) < 0) {
            exit(EXIT_FAILURE);
        }
        std_out_channels.push_back(p_out);
        std_err_channels.push_back(p_err);
    }

    for (int i = 0; i < pipeModel.commands.size(); i++) {
        Command shellCommand = pipeModel.commands[i];
        int *current_out_fd = std_out_channels[i].data();
        int *current_err_fd = std_err_channels[i].data();
        int *prev_out_fd = i == 0 ? nullptr : std_out_channels[i - 1].data();

        std::string programName = StringUtil::convertToCppStyleString(shellCommand.argv[0]);

        pid_t c_pid = fork();
        if (c_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (c_pid == 0) {
            // CHILD

            dup2(current_out_fd[1], STDOUT_FILENO);
            dup2(current_err_fd[1], STDERR_FILENO);

            if (prev_out_fd != nullptr) {
                dup2(prev_out_fd[0], STDIN_FILENO);
                close(prev_out_fd[0]);
            }

            for (int j = 0; j < std_out_channels.size(); j++) {
                close(current_out_fd[j]);
                close(current_err_fd[j]);
            }

            std::string executablePath = resolveExecutablePath(
                StringUtil::convertToCppStyleString(shellCommand.argv[0]));

            if (executablePath.empty()) {
                std::cerr <<  shellCommand.argv[0] << " executable not found" << std::endl;
            }

            execve(StringUtil::convertToCString(executablePath), shellCommand.argv.data(), envp.data());

            perror("execve");
            _exit(126);
        }

        close(current_out_fd[1]);
        close(current_err_fd[1]);
        pids.push_back(c_pid);

    }


    auto readStdOut = [&std_out_channels, &std_outs](int i) -> void {
        size_t n;
        char outbuf[MSGSIZE];
        std::string stdOut = "";
        int *stdOutPipe = std_out_channels[i].data();
        while ((n = read(stdOutPipe[0], outbuf, MSGSIZE)) > 0) {
            stdOut.append(outbuf, n);
        }
        close(stdOutPipe[0]);
        std_outs[i] = stdOut;
    };

    auto readStdErr = [&std_err_channels, &std_errs](int i) -> void {
        size_t n;
        char outbuf[MSGSIZE];
        std::string stdErr = "";
        int *stdErrPipe = std_err_channels[i].data();
        while ((n = read(stdErrPipe[0], outbuf, MSGSIZE)) > 0) {
            stdErr.append(outbuf, n);
        }
        close(stdErrPipe[0]);
        std_errs[i] = stdErr;
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < pipeModel.commands.size(); i++) {
        std::thread t_std_out(readStdOut, i);
        std::thread t_std_err(readStdErr, i);
        threads.push_back(std::move(t_std_out));
        threads.push_back(std::move(t_std_err));
    }

    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    for (int i = 0; i < pids.size(); i++) {
        int status;
        waitpid(pids[i], &status, 0);
        exit_status[i] = status;
    }

    // last exit status
    int lastProcessExitStatus = exit_status[pipeModel.commands.size() - 1];
    std::string mergedErrorMessages;
    for (auto i : std_errs) {
        if (!i.second.empty()) {
            mergedErrorMessages += i.second + " \n";
        }
    }

    ExecuteProcessResult result;
    result.stdOut = std_outs[pipeModel.commands.size() - 1];
    result.stdErr = mergedErrorMessages;

    if ( WIFEXITED(lastProcessExitStatus) )
    {
        int lastExitStatus = WEXITSTATUS(lastProcessExitStatus);

        result.exitCode= lastExitStatus;
        result.signaled = false;
        result.exitedNormally = true;
    }
    else if (WIFSIGNALED(lastProcessExitStatus)) {
        int lastExitStatus = 128 + WTERMSIG(lastProcessExitStatus);
        int sig = WTERMSIG(lastProcessExitStatus);

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
