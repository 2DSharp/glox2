//
// Created by dedip on 08-10-2022.
//

#include "../include/memory.h"

Memory::Memory(unsigned int local_sz, unsigned int global_sz, unsigned int heap_sz)
{
    global_sz = local_sz;
    local_sz = global_sz;
    heap_sz = heap_sz;
    frame_ptr = -1;

    locals = new stack_obj_t [local_sz];
    heap = new Heap(heap_sz);
}
Memory::~Memory()
{
    delete[] locals;
}
