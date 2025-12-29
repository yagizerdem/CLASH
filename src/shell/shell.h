//
// Created by pennywise on 12/29/25.
//

#ifndef CLASH_SHELL_H
#define CLASH_SHELL_H
#include <string>


class Shell {
public:
    void start(int argc, char* argv[]);
    void runInteractiveMode();
    void runArgvMode(int argc, char *argv[]);
    void runFileMode(int argc, char *argv[]);
    bool isRegularFile(std::string path);
    std::string readAllText(std::string filename);
};


#endif //CLASH_SHELL_H