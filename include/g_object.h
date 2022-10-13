//
// Created by dedip on 13-10-2022.
//

#ifndef GLOX_G_OBJECT_H
#define GLOX_G_OBJECT_H


#include "stack.h"
#define G_ARRAY_OBJ 1
#define G_ARRAY_BOOL 2
#define G_ARRAY_INT 3
#define G_ARRAY_FLOAT 4
#define G_ARRAY_CHAR 5

class GObject {


public:
    struct array_descriptor_t {
        size_t size;
        unsigned int type;
    };
    struct container_obj_t {
        size_t size;
    };
    typedef enum type {
        PRIMITIVE = 0,
        ARRAY = 1,
        OBJ = 2
    } Type;

    GObject(){};
    GObject(stack_obj_t * data, Type type);
    GObject(container_obj_t * container, Type type);
    GObject(array_descriptor_t * array_descriptor, Type type);


private:
    container_obj_t * _container;
    array_descriptor_t * _array_descriptor;
    stack_obj_t * _data;
    Type _type;
};


#endif //GLOX_G_OBJECT_H
