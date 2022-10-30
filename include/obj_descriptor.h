//
// Created by dedip on 23-10-2022.
//

#ifndef GLOX_OBJ_DESCRIPTOR_H
#define GLOX_OBJ_DESCRIPTOR_H


#include "g_object.h"
#include "ClassDef.h"

class ObjectDescriptor : public GObject {
private:
    GObject **_fields;
    ClassDef *context;

public:
    explicit ObjectDescriptor(ClassDef *class_def);

    // GMethod ** _methods
    void set_field(int index, GObject *obj);

    GObject *get_field(int index);

    ClassDef *get_context();

    ~ObjectDescriptor() override;

};


#endif //GLOX_OBJ_DESCRIPTOR_H
