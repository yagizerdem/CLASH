//
// Created by pennywise on 12/26/25.
//

#ifndef CLASH_COMMANDSPLITTER_H
#define CLASH_COMMANDSPLITTER_H
#include <variant>
#include <vector>
#include <string>


#include "../util/model/command.h"
#include "../util/model/pipe.h"

class CommandSplitter {
public:
    std::vector<std::variant<Command, Pipe>>  commandStream(std::string rawShellCommand);

private:
    int lookAheadIndex;
    char peek();
    char move();
    std::string rawShellCommand;
    std::string processCommand();
    std::string processDoubleQuote();
    std::string processSingleQuote();
    std::string processBackTick();
    std::string processUnquoted();
    int getSplitterPresedence(char splitter);
    struct CommandSegment {
        std::string token;
        bool isSplitter;
        bool isCommand;
    };
};


#endif //CLASH_COMMANDSPLITTER_H