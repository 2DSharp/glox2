//
// Created by dedip on 09-10-2022.
//

#include "io.h"

void print(std::string str) {
    std::cout << str;
}
void GNative_io_print(GRuntime * runtime, std::string str) {
    GClass str_class = runtime->get_class("glox/math/type/Point");
//    GObject obj = runtime->init_new(str_class, 10);
   // int x = runtime->invoke(obj, "getX");
   // int y = runtime->invoke(obj, "getY");
    std::cout << "x " << str_class.class_name;
    print(str);
}
