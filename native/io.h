//
// Created by dedip on 09-10-2022.
//

#ifndef GLOX_BASICIO_H
#define GLOX_BASICIO_H

#include "gnative.h"
#include <string>
#include <iostream>


extern "C"
{

void GNative_io_print(std::string str);
}
void _invoke_gnative_function(GNative_OBJ* parameters, std::string func) {
    if (func == "GNative_io_print") {
        if (parameters->type == INT) {
            GNative_io_print(std::to_string(parameters[0].val.n));
        }
        if (parameters->type == CHAR) {
            std::string str{parameters[0].val.s};
            GNative_io_print(str);
        }
    }
}
#endif //GLOX_BASICIO_H
