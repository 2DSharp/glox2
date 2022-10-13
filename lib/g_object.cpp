//
// Created by dedip on 13-10-2022.
//

#include "../include/g_object.h"

GObject::GObject(stack_obj_t * data, GObject::Type type) : _data(data), _type(type)
{
}

GObject::GObject(GObject::container_obj_t * data, GObject::Type type) :  _container(data), _type(type)
{
}

GObject::GObject(GObject::array_descriptor_t *array_descriptor, GObject::Type type) : _array_descriptor(array_descriptor), _type(type) {

}
