//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_EXEC_H
#define GLOX_EXEC_H

#include "stack.h"
#include "code.h"
#include "memory.h"
#include "function.h"
#include "opcode.h"
#include "bytecode.h"
#include "dl_loader.h"
#include <iostream>

void exec_nop();

void exec_iadd(Stack *stack);

void exec_isub(Stack *stack);

void exec_imul(Stack *stack);

void exec_idiv(Stack *stack);

short exec_iconst(Stack *stack, Code *code, short ip);

void exec_print(Stack *stack);

void exec_println(Stack *stack);

short exec_load(Stack *stack, const Code *code, short ip, Memory *mem);

short exec_store(Stack *stack, const Code *code, short ip, Memory *mem);

short exec_jmp(const Code *code, short ip);

short exec_jmpt(Stack *stack, const Code *code, short ip);

void exec_ieq(Stack *stack);

void exec_ilt(Stack *stack);

void exec_pop(Stack *stack);

short exec_call(Stack *stack, const Code *code, short ip, Memory *mem, const Function *fn_pool, int *caller_index);

short exec_ret(Stack *stack, const Code *code, short ip, Memory *mem, const Function *fn_pool, int *caller_index);

void opcode_runner_init(Opcode *ops);

#endif //GLOX_EXEC_H
