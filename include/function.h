//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_FUNCTION_H
#define GLOX_FUNCTION_H

#include <string>
#include "code.h"

class Function {
public:
    int locals;
    int n_args;
    short addr;
    int return_type;
    enum {GENERAL, NATIVE} func_type;
    std::string lib_path;
    std::string call_symbol;
    Code code = Code(nullptr);


} ;

#endif //GLOX_FUNCTION_H
