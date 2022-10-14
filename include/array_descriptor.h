//
// Created by dedip on 14-10-2022.
//

#ifndef GLOX_ARRAY_DESCRIPTOR_H
#define GLOX_ARRAY_DESCRIPTOR_H


#include "g_object.h"

class ArrayDescriptor : public GObject {
public:
    ArrayDescriptor(size_t size, unsigned int type);
    size_t _size;
    unsigned int _type;
    addr get_address_from_index(int index);
};


#endif //GLOX_ARRAY_DESCRIPTOR_H
