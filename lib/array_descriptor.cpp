//
// Created by dedip on 14-10-2022.
//

#include "../include/array_descriptor.h"

addr ArrayDescriptor::get_address_from_index(int index) {
    return _address + index;
}

ArrayDescriptor::ArrayDescriptor(size_t size, unsigned int type) : _size(size), _type(type), GObject(ARRAY) {
}
