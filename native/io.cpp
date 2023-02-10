//
// Created by dedip on 09-10-2022.
//

#include "io.h"

void print(std::string str) {
    std::cout << str;
}

void GNative_io_print(GRuntime *runtime, GNativeString str) {
    const char * chars = runtime->get_str_chars(str);
    std::string chars_to_str(chars);

//    GClass point_cls = runtime->get_class("glox/math/type/Point");
//    GParamList params = runtime->init_params(1, GNativeObj{.type = INT, .val = {.n = 77}});
//    GNativeObj obj;
//    obj = runtime->init_new(point_cls, params);
//    std::cout << "OBJ: " << obj.val.addr;
//    params = runtime->init_params(0);
//    GNativeObj x = runtime->invoke(obj, "getX", params);
//
//    std::cout << "x " << x.val.addr << " ";
    print(chars_to_str);
}
