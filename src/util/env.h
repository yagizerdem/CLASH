//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_ENV_H
#define CLASH_ENV_H
#include <string>
#include <unordered_map>
#include "./model/variable.h"
#include <mutex>

class Env {

private:
    static Env* instancePtr;

    // Mutex to ensure thread safety
    static std::mutex mtx;

    // Private Constructor
    Env() {}

public:
    Env(const Env& obj) = delete;

    // Static method to get the Singleton instance
    static Env* getInstance() {
        if (instancePtr == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (instancePtr == nullptr) {
                instancePtr = new Env();
            }
        }
        return instancePtr;
    }

    std::unordered_map<std::string, Variable> environment_variables;
    std::unordered_map<std::string, std::string> PATH_CACHE;

    void loadEnvp(char* envp[]);

    void setEnv(std::string key, std::string value);
    void unsetEnv(std::string key);
    Variable getEnv(std::string key);
    void exportEnv(std::string key);

    void setCwd(std::string value);
    std::string getCwd();
};


#endif //CLASH_ENV_H