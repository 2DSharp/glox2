//
// Created by dedip on 13-10-2022.
//

#include <iostream>
#include "../include/Heap.h"

Heap::Heap(unsigned int size) {
    _size = size;
    GObject * objects[_size];
    _objects = objects;
    _tail = 0;

}

addr Heap::allocate_contiguous_block(GObject * object, int size) {
    _objects[_tail] = object;
    object->set_address(_tail);
    addr curr = _tail;
    ++_tail;
    _tail += size;
    return curr;
};

GObject * Heap::get_object(addr address) {
    return _objects[address];
}

bool Heap::alloc(GObject *obj) {
    _objects[_tail] = obj;
    ++_tail;
    return true;
}

bool Heap::alloc(addr address, GObject *obj) {
    _objects[address] = obj;
    obj->set_address(address);
    return true;
}
