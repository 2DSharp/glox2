//
// Created by dedip on 13-10-2022.
//

#ifndef GLOX_G_OBJECT_H
#define GLOX_G_OBJECT_H


#include "stack.h"
#include "constants.h"

#define G_ARRAY_OBJ 1
#define G_ARRAY_BOOL 2
#define G_ARRAY_INT 3
#define G_ARRAY_FLOAT 4
#define G_ARRAY_CHAR 5

class GObject {
public:
    addr _address;

    struct container_obj_t {
        size_t size;
    };
    typedef enum type {
        PRIMITIVE = 0,
        ARRAY = 1,
        OBJ = 2
    } Type;

    GObject() = default;

    GObject(stack_obj_t *data, Type type);

    explicit GObject(Type type);


    void set_address(addr address);

    stack_obj_t _data;
    Type _type;
    virtual ~GObject() = default;
};


#endif //GLOX_G_OBJECT_H
