//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_MEMORY_H
#define GLOX_MEMORY_H
#include <algorithm>
#include "stack.h"

class Memory
{
public:
    stack_obj_t * locals;
    unsigned int local_sz;
    short * globals;
    unsigned int global_sz;
    short frame_ptr;

    Memory(unsigned int local_sz, unsigned int global_sz);
    ~Memory();
} ;

#endif //GLOX_MEMORY_H
