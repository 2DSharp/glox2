//
// Created by dedip on 19-02-2023.
//

#ifndef GLOX_SERIALIZATION_H
#define GLOX_SERIALIZATION_H


#include "../lib/vm.h"

class Serialization {
public:
    void save_state(VM * vm, const char * location);
};


#endif //GLOX_SERIALIZATION_H
