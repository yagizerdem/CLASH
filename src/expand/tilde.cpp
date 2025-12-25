//
// Created by pennywise on 12/16/25.
//

#include "tilde.h"
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <vector>
#include <bits/error_constants.h>

#include "../util/stringUtil.h"

enum State {
    NORMAL,
    IN_SINGLE_QUOTE,
    IN_DOUBLE_QUOTE
};

std::string Tilde::expandTilde(std::string rawCommand) {
    if (rawCommand.empty()) return rawCommand;

    State state = NORMAL;
    bool atWordStart = true;

    for (size_t i = 0; i < rawCommand.size(); i++) {
        char ch = rawCommand[i];

        if (ch == '\'' && !StringUtil::isEscapedCharacter(i, rawCommand)) {
            state = (state == IN_SINGLE_QUOTE) ? NORMAL : IN_SINGLE_QUOTE;
            atWordStart = false;
            continue;
        }

        if (ch == '"' && !StringUtil::isEscapedCharacter(i, rawCommand)) {
            state = (state == IN_DOUBLE_QUOTE) ? NORMAL : IN_DOUBLE_QUOTE;
            atWordStart = false;
            continue;
        }

        if (isspace(ch)) {
            atWordStart = true;
            continue;
        }

        if (ch == '~' && atWordStart && state == NORMAL) {
            size_t j = i + 1;
            std::string user;

            while (j < rawCommand.size() &&
                   rawCommand[j] != '/' &&
                   !isspace(rawCommand[j])) {
                user.push_back(rawCommand[j]);
                j++;
                   }

            std::string homeDir;
            if (user.empty()) {
                homeDir = getHomeDirCurrentUser();
            } else {
                homeDir = getHomeDirByUsername(user);
            }

            if (!homeDir.empty()) {
                rawCommand.replace(i, 1 + user.size(), homeDir);
                i += homeDir.size() - 1;
            }

            atWordStart = false;
            continue;
        }

        atWordStart = false;
    }

    return rawCommand;
}

std::string Tilde::getHomeDirByUsername(std::string username) {
    struct passwd pwd;
    struct passwd* result = nullptr;
    std::vector<char> buffer(16384);

    int ret = getpwnam_r(
        username.c_str(),
        &pwd,
        buffer.data(),
        buffer.size(),
        &result
    );

    if (ret != 0 || !result || !pwd.pw_dir) {
        return "";
    }

    return std::string(pwd.pw_dir);
}

std::string Tilde::getHomeDirCurrentUser() {
    struct passwd pwd;
    struct passwd* result = nullptr;
    std::vector<char> buffer(16384);

    if (getpwuid_r(getuid(), &pwd,
                   buffer.data(), buffer.size(),
                   &result) == 0 &&
        result && pwd.pw_dir) {
        return std::string(pwd.pw_dir);
        }

    if (const char* home = getenv("HOME")) {
        return std::string(home);
    }

    return "";
}

