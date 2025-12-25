//
// Created by pennywise on 12/25/25.
//

#include "env.h"

#include <iostream>
#include "stringUtil.h"

Env* Env::instancePtr = nullptr;
std::mutex Env::mtx;

void Env::loadEnvp(char* envp[]) {

    for ( int i = 0; envp[i] != nullptr; ++i )
    {
        std::string env_var = StringUtil::convertToCppStyleString(envp[i]);

        int j = 0;
        std::string key , value;
        while (env_var[j] != '=') {
            key += env_var[j];
            j++;
        }
        j++;
        while (j < env_var.length()) {
            value += env_var[j];
            j++;
        }
        this->setEnv(key, value);
    }
}

void Env::exportEnv(std::string key) {
    this->environment_variables[key].isExported = true;
}

Variable Env::getEnv(std::string key) {
    return this->environment_variables[key];
}

void Env::unsetEnv(std::string key) {
    this->environment_variables.erase(key);
}

void Env::setEnv(std::string key, std::string value) {
    Variable var;
    var.value = value;
    var.key = key;
    var.isExported = false;
    this->environment_variables[key] = var;
}
