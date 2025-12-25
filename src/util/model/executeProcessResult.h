//
// Created by pennywise on 12/25/25.
//

#ifndef CLASH_EXECUTEPROCESSRESULT_H
#define CLASH_EXECUTEPROCESSRESULT_H
#include <string>

class ExecuteProcessResult {
public:
    std::string stdOut;
    std::string stdErr;

    bool exitedNormally = false;
    bool signaled = false;

    int exitCode = -1;
    int signal = 0;
};

#endif //CLASH_EXECUTEPROCESSRESULT_H