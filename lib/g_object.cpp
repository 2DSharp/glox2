//
// Created by dedip on 13-10-2022.
//

#include "../include/g_object.h"

GObject::GObject(stack_obj_t * data, GObject::Type type) : _data(*data), _type(type)
{
}


void GObject::set_address(addr address) {
    _address = address;
}

GObject::GObject(GObject::Type type):  _type(type) {

}
