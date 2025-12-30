//
// Created by pennywise on 12/29/25.
//

#ifndef CLASH_ENGINERESPONSE_H
#define CLASH_ENGINERESPONSE_H

class  EngineResponse {
public:
    std::vector<std::string> payload;
    std::string errorMessage;
    bool success;
    int lastCommandExitStatus;
    bool terminate;
    bool interactiveContinue;
};

#endif //CLASH_ENGINERESPONSE_H