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


class Exec {
private:

    Stack *stack;
    Code *code;
    Memory *mem;
    Function *fn_pool;
    std::map<std::string, ClassDef*> *loaded_classes;
    short call_fn(short target_index, short ip, short *caller_index);
    Bytecode code_fetch(short e_index);
    ClassDef *get_class_context(const std::string& class_name);

public:
    typedef void (Exec::*op_none)(void);

    typedef void (Exec::*op_noargs)(void);
    typedef short (Exec::*op_args)(short ip);
    typedef short (Exec::*op_caller)(short ip, short * caller_index);

    typedef enum op_type {
        NONE,
        NOARGS,
        WITH_ARGS,
        CALLER,
        INVALID
    } OpType;
    typedef struct opcode_t
    {
        short opcode;
        OpType type;
        union {
            op_none exec_none;
            op_noargs exec_noargs;
            op_args exec_args;
            op_caller exec_caller;
        };
    } Opcode;
    Exec(Stack *stack, Code *code, Memory *mem, Function *fn_pool, std::map<std::string, ClassDef*> *loaded_classes);
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

    short e_call(short ip,short *caller_index);

    short e_ret(short ip,short *caller_index);

    short e_newparray(short ip);

    short e_paload(short ip);

    short e_alen(short ip);

    short e_pastore(short ip);

    void opcode_runner_init(Opcode *ops);

    short e_set(short ip);

    short e_new(short ip);

    short e_ocall(short ip, short *caller_index);

    short e_cload(short ip, short *caller_index);

    short e_get(short ip);

    short ocall_lib(const char *library_func, short ip, short *caller_index);

    void instantiate_object(const std::string& class_name);

    GClass native_get_class(const char *cls);
};

#endif //GLOX_H
