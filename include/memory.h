//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_MEMORY_H
#define GLOX_MEMORY_H
#include <algorithm>
#include "stack.h"
#include "Heap.h"
#include "Constant.h"

class Memory
{
public:

    short * globals;
    unsigned int global_sz;
    Constant ** constant_pool;
    Heap * heap;
    Memory(Constant * local_sz[], unsigned int global_sz, unsigned int heap_sz);
    ~Memory();
} ;

#endif //GLOX_MEMORY_H
