//
// Created by dedip on 11-10-2022.
//

#ifndef GLOX_GNATIVE_H
#define GLOX_GNATIVE_H

#include <string>
#define OP 0
#define INT 1
#define CHAR 2
#define FLOAT 3
#define BOOL 4
#define ADDR 5
#define STR 6

typedef union {
    short op;
    short addr;
    int n;
    char s;
    float f;
    bool b;
} GNative_Data;
typedef struct obj_t
{
    unsigned int type;
    GNative_Data val;
} GNative_OBJ;

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
    const char * class_name;
} GClass;

typedef struct glox_class_obj_t {
    GNative_OBJ addr;
} GNativeObject;

typedef struct glox_runtime_t {
    GClass (*get_class)(const char * name) = nullptr;
    void (*invoke)(GNativeObject obj) = nullptr;
    GNativeObject (*init_new)(GClass cls,...) = nullptr;

} GRuntime;

extern "C"
{
void* _invoke_gnative_function(GNative_OBJ* parameters, std::string func);
void _initialize_glox_runtime(GRuntime g_runtime);
}
#endif //GLOX_GNATIVE_H
