//
// Created by dedip on 14-10-2022.
//

#ifndef GLOX_GOBJECTFACTORY_H
#define GLOX_GOBJECTFACTORY_H


#include "g_object.h"
class GObjectFactory {
public:
    static GObject *create_array_descriptor(size_t size, unsigned int type);
}

#endif //GLOX_GOBJECTFACTORY_H
