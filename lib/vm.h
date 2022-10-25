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
#include "../include/opcode.h"
#include "../include/stack.h"
#include "../include/exec.h"
#include <iostream>

class VM {
public:
    VM(size_t stack_size, Memory * mem);
    void vm_run(const Function * func_pool, short func_index, int debug);
    void vm_close();
    void vm_exec(Code * code_mem, const Opcode * opcode, const Function * func_pool, short * func_index);

private:
    typedef enum state_t
    { ST_RUNNING, ST_HALTED, ST_INVALID } State;

    short instr_ptr;
    Stack * stack;
    Memory * memory;
    State state;
    static short get_code(Bytecode code);

};


#endif //GLOX_VM_H
