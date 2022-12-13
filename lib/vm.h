//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_VM_H
#define GLOX_VM_H


#include <cstdlib>
#include "../include/memory.h"
#include "../include/code.h"
#include "../include/bytecode.h"
#include "../include/function.h"
#include "../include/stack.h"
#include "../include/exec.h"
#include <iostream>

class VM {
public:
    VM(size_t stack_size, Memory *mem, std::map<std::string, ClassDef*> *loaded_classes, Function * func_pool, int debug);
    void vm_run(short func_index);
    void vm_close();

private:
    typedef enum state_t
    { ST_RUNNING, ST_HALTED, ST_INVALID } State;

    short instr_ptr;
    Stack * stack;
    Memory * memory;
    State state;
    Function *func_pool;
    int debug = 0;
    static short get_code(Bytecode code);


    void vm_exec(Exec exec, const Exec::Opcode *opcode, short *func_index);

    std::map<std::string, ClassDef *> *loaded_classes;
    GNativeObj vm_native_callback(short * func_index, short ip);

    void vm_run(short * func_index, bool temp_spawn);
};



#endif //GLOX_VM_H
