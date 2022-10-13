//
// Created by dedip on 14-10-2022.
//

#include "../include/g_object_factory.h"


GObject *GObjectFactory::create_array_descriptor(size_t size, unsigned int type) {
    GObject::array_descriptor_t container = {.size = size, .type = type};
    auto *obj = new GObject(&container, GObject::ARRAY);
    return obj;
}
