//
// Created by dedip on 13-10-2022.
//

#include "../include/Heap.h"

Heap::Heap(unsigned int size) {
    GObject * objects[size];
    _size = size;
    _objects = objects;
    _tail = 0;

}

addr Heap::allocate_contiguous_block(GObject * object, int size) {
    _objects[_tail] = object;
    _tail += size;
    return _tail;
};
