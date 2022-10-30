//
// Created by dedip on 23-10-2022.
//

#ifndef GLOX_CONSTANT_H
#define GLOX_CONSTANT_H


#include "constants.h"

class Constant {
public:


    enum Type {
        STRING,
        FUNC,
        CLASS
    } type;

    Constant(Type type, const char * val);


    const char * val;
    addr ref;
};


#endif //GLOX_CONSTANT_H
