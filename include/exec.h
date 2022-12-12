//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_H
#define GLOX_H

#include "stack.h"
#include "code.h"
#include "memory.h"
#include "function.h"
#include "bytecode.h"
#include "dl_loader.h"
#include "g_object_factory.h"
#include "../native/gnative.h"
#include <iostream>
#include <functional>
#include <cstring>
typedef std::function<GNativeObj(short,short)> vm_run_callback;

class Exec {
private:

    Stack *stack;
    Code *code;
    Memory *mem;
    Function *fn_pool;
    std::map<std::string, ClassDef *> *loaded_classes;
    static stack_obj_t convert_native_to_stack_obj(GNativeObj obj) {
        auto * res = (stack_obj_t *) &obj;
        return *res;
    }

    static GNativeObj convert_stack_obj_to_native(stack_obj_t obj) {
        auto * res = (GNativeObj *) &obj;
        return *res;
    }

    short call_fn(short target_index, short ip, short *caller_index, vm_run_callback runner);

    Bytecode code_fetch(short e_index);

    ClassDef *get_class_context(const std::string &class_name);


    GClass native_get_class(const char *cls);

    GNativeObj native_invoke(short ip, short *caller_index, vm_run_callback callback, GNativeObj obj, const char *func,
                             GParamList params_meta);
    GNativeObj native_instantiate_obj(short ip, short *caller_index, vm_run_callback, GClass cls, GParamList params);

public:
    typedef void (Exec::*op_none)(void);

    typedef void (Exec::*op_noargs)(void);

    typedef short (Exec::*op_args)(short ip);
    typedef short (Exec::*op_caller)(short ip, short *caller_index, vm_run_callback);

    typedef enum op_type {
        NONE,
        NOARGS,
        WITH_ARGS,
        CALLER,
        INVALID
    } OpType;
    typedef struct opcode_t {
        short opcode;
        OpType type;
        union {
            op_none exec_none;
            op_noargs exec_noargs;
            op_args exec_args;
            op_caller exec_caller;
        };
    } Opcode;

    Exec(Stack *stack, Code *code, Memory *mem, Function *fn_pool, std::map<std::string, ClassDef *> *loaded_classes);

    void e_nop();

    void e_iadd();

    void e_isub();

    void e_imul();

    void e_idiv();

    short e_iconst(short ip);

    void e_print();

    void e_println();

    short e_load(short ip);

    short e_store(short ip);

    short e_jmp(short ip);

    short e_jmpt(short ip);

    void e_ieq();

    void e_ilt();

    void e_pop();

    short e_call(short ip, short *caller_index, vm_run_callback);

    short e_ret(short ip, short *caller_index, vm_run_callback);

    short e_newparray(short ip);

    short e_paload(short ip);

    short e_alen(short ip);

    short e_pastore(short ip);

    void opcode_runner_init(Opcode *ops);

    short e_set(short ip);

    short e_new(short ip);

    short e_ocall(short ip, short *caller_index, vm_run_callback callback);

    short e_cload(short ip, short *caller_index, vm_run_callback);

    short e_get(short ip);

    short ocall_lib(const char *library_func, short ip, short *caller_index, const vm_run_callback&);

    void instantiate_object(const std::string &class_name);

};

#endif //GLOX_H
