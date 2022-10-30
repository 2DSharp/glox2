//
// Created by dedip on 14-10-2022.
//

#ifndef GLOX_GOBJECTFACTORY_H
#define GLOX_GOBJECTFACTORY_H


#include "g_object.h"
#include "array_descriptor.h"
#include "obj_descriptor.h"

class GObjectFactory {
public:
    static ArrayDescriptor * create_array_descriptor(size_t size, unsigned int type);
    static GObject * create_primitive_object(stack_obj_t * data);
    static ObjectDescriptor *create_object_descriptor(ClassDef *context);
};

#endif //GLOX_GOBJECTFACTORY_H
