//
// Created by dedip on 09-10-2022.
//

#ifndef GLOX_BASICIO_H
#define GLOX_BASICIO_H

#include "gnative.h"
#include <string>
#include <iostream>

void (*callback)() = NULL;
GRuntime runtime;

extern "C"
{
void GNative_io_print(GRuntime * runtime, std::string str);
}

void _initialize_glox_runtime(GRuntime g_runtime) {
    runtime = g_runtime;
}

void* _invoke_gnative_function(GNativeObj * parameters, std::string func) {
    if (func == "GNative_io_print") {
        if (parameters->type == INT) {
            GNative_io_print(&runtime, std::to_string(parameters[0].val.n));
        }
        if (parameters->type == CHAR) {
            std::string str{parameters[0].val.c};
            GNative_io_print(&runtime, str);
        }
    }
}
void _register_func(void (*callback_func)()) {

}
#endif //GLOX_BASICIO_H
