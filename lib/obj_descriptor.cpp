//
// Created by dedip on 23-10-2022.
//

#include "../include/obj_descriptor.h"

void ObjectDescriptor::set_field(int index, GObject *obj) {
    _fields[index] = obj;
}

ObjectDescriptor::ObjectDescriptor(ClassDef* class_def) : context(class_def), GObject(GObject::OBJ) {
    _fields = new GObject*[context->get_num_fields()];
}

ObjectDescriptor::~ObjectDescriptor() {
    delete[] _fields;
}

GObject * ObjectDescriptor::get_field(int index) {
    return _fields[index];
}

ClassDef *ObjectDescriptor::get_context() {
    return context;
}
