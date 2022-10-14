//
// Created by dedip on 13-10-2022.
//

#ifndef GLOX_HEAP_H
#define GLOX_HEAP_H

#include "g_object.h"
#include "constants.h"

class Heap {
private:
    unsigned int _size;
    GObject ** _objects;
    addr _tail;

public:
    explicit Heap(unsigned int size);
    addr allocate_contiguous_block(GObject *object, int size);

    GObject *get_object(addr address);

    bool alloc(addr address, GObject * obj);
    bool alloc(GObject * obj);
};


#endif //GLOX_HEAP_H
