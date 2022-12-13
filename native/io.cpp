//
// Created by dedip on 09-10-2022.
//

#include "io.h"

void print(std::string str) {
    std::cout << str;
}

void GNative_io_print(GRuntime *runtime, std::string str) {
    GClass str_class = runtime->get_class("glox/math/type/Point");
    GParamList params = runtime->init_params(1, GNativeObj{.type = INT, .val = {.n = 77}});
    GNativeObj obj;
    obj = runtime->init_new(str_class, params);
    std::cout << "OBJ: " << obj.val.addr;
    params = runtime->init_params(0);
    GNativeObj x = runtime->invoke(obj, "getX", params);

    std::cout << "x " << x.val.addr << " ";
    print(str);
}
