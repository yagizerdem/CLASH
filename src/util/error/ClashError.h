//
// Created by pennywise on 12/29/25.
//

#ifndef CLASH_CLASHERROR_H
#define CLASH_CLASHERROR_H

class ClashError : public std::exception {
public:
    explicit ClashError(std::string msg)
        : message(std::move(msg)) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
protected:
    std::string message;
};

#endif //CLASH_CLASHERROR_H