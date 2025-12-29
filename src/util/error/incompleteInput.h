//
// Created by pennywise on 12/29/25.
//

#ifndef CLASH_INCOMPLETEINPUT_H
#define CLASH_INCOMPLETEINPUT_H
#include "clashError.h"


class IncompleteInput : public ClashError {
public:
    using ClashError::ClashError;
};

#endif //CLASH_INCOMPLETEINPUT_H