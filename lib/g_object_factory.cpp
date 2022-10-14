//
// Created by dedip on 14-10-2022.
//

#include "../include/g_object_factory.h"


ArrayDescriptor * GObjectFactory::create_array_descriptor(size_t size, unsigned int type) {
    return new ArrayDescriptor(size, type);
}

GObject *GObjectFactory::create_primitive_object(stack_obj_t *data) {
    return new GObject(data, GObject::PRIMITIVE);
}
