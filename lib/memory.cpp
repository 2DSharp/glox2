//
// Created by dedip on 08-10-2022.
//

#include "../include/memory.h"

Memory::Memory(Constant * const_pool[], unsigned int global_sz, unsigned int heap_sz)
{
    constant_pool = const_pool;
    heap_sz = heap_sz;

    heap = new Heap(heap_sz);
}
Memory::~Memory()
{
    //delete[] constant_pool;
}
