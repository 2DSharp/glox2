//
// Created by dedip on 11-10-2022.
//

#ifndef GLOX_GNATIVE_H
#define GLOX_GNATIVE_H

#include <cstdarg>

#include <string>

#define OP 0
#define INT 1
#define CHAR 2
#define FLOAT 3
#define BOOL 4
#define ADDR 5
#define STR 6

typedef union {
    short addr;
    int n;
    char c;
    float f;
    bool b;
} GNative_Data;
typedef struct obj_t
{
    unsigned int type;
    GNative_Data val;
} GNativeObj;

//class GloxRuntime {
//public:
//    class Object {
//        short addr;
//    };
//    void invoke(Object instance, const std::string& method, args) {
//
//    }
//    T get_field(instance, std::string field_name);
//    set_field(instance, std::string field_name, value);
//    T create_new(class_name);
//};

typedef struct glox_class_t {
    char *class_name;
} GClass;

struct GParamList {
    GNativeObj *params;
    size_t size;
};

inline GParamList g_init_params(size_t sz, ...) {
    auto *params = (GNativeObj *) malloc(sizeof(GNativeObj) * sz);
    va_list ptr;


    va_start(ptr, sz);
    for (int i = 0; i < sz; i++) {
        params[i] = va_arg(ptr, GNativeObj);
    }
    GParamList list = {
            .params = params,
            .size = sz
    };

    return list;
}

typedef struct glox_runtime_t {
    GClass (*get_class)(const char *name) = nullptr;

    GNativeObj (*invoke)(GNativeObj obj, const char *func_name, GParamList) = nullptr;

    GNativeObj (*init_new)(GClass cls, GParamList) = nullptr;

    GParamList (*init_params)(size_t sz, ...) = g_init_params;

} GRuntime;

extern "C"
{
void *_invoke_gnative_function(GNativeObj *parameters, std::string func);
void _initialize_glox_runtime(GRuntime g_runtime);
}
#endif //GLOX_GNATIVE_H
