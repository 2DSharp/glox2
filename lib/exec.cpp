#include "../include/exec.h"

//#define  __get_data(bytecode, type) (*(type *) bytecode.val)


void exec_nop() {
    return;
}

stack_obj_t stack_pop(Stack *stack) {
    return stack->pop();
}

Bytecode code_fetch(Code *code, short index) {
    return code->code_fetch(index);
}

int stack_push(Stack *stack, stack_obj_t val) {
    return stack->push(val);
}

void exec_iadd(Stack *stack) {
    stack_obj_t b = stack_pop(stack);
    stack_obj_t a = stack_pop(stack);
    stack_obj_t res;
    res.type = INT;
    res.val.n = a.val.n + b.val.n;
    stack_push(stack, res);
}

void exec_isub(Stack *stack) {
    short b = stack_pop(stack).val.n;
    short a = stack_pop(stack).val.n;

    stack_obj_t res;
    res.type = INT;
    res.val.n = a - b;
    stack_push(stack, res);
}

void exec_imul(Stack *stack) {
    short b = stack_pop(stack).val.n;
    short a = stack_pop(stack).val.n;

    stack_obj_t res;
    res.type = INT;
    res.val.n = a * b;
    stack_push(stack, res);
}

void exec_idiv(Stack *stack) {
    short b = stack_pop(stack).val.n;
    short a = stack_pop(stack).val.n;

    stack_obj_t res;
    res.type = INT;
    res.val.n = a / b;

    stack_push(stack, res);
}

short exec_iconst(Stack *stack, Code *code, short ip) {
    stack_obj_t v = code->code_fetch(++ip);
    stack_push(stack, v);
    return ++ip;
}

void exec_print(Stack *stack) {
    stack_obj_t v = stack_pop(stack);
    switch (v.type) {
        case INT:
            std::cout << v.val.n;
            break;
        case BOOL:
            std::cout << v.val.b;
            break;
        case CHAR:
            std::cout << v.val.s;
            break;
        case FLOAT:
            std::cout << v.val.f;
            break;
    }
    /* We don't want to remove this from the stack */
    stack_push(stack, v);
}

/* Temporary till we start supporting ASCII prints */
void exec_println(Stack *stack) {
    exec_print(stack);
    printf("\n");
}

short exec_load(Stack *stack, Code *code, short ip, Memory *mem) {
    short offset = code_fetch(code, ++ip).val.addr;
    short addr = offset + mem->frame_ptr;
    stack_push(stack, mem->locals[addr]);
    return ++ip;
}


short exec_store(Stack *stack, Code *code, short ip, Memory *mem) {

    short offset = code_fetch(code, ++ip).val.addr;
    mem->locals[mem->frame_ptr + offset] = stack_pop(stack);
    return ++ip;
}

short exec_jmp(Code *code, short ip) {
    return (code_fetch(code, ++ip).val.n);
}

/*
 * Conditional Jump
 * If true on top of stack
 */
short exec_jmpt(Stack *stack, Code *code, short ip) {
    short addr = code_fetch(code, ++ip).val.addr;
    if (stack_pop(stack).val.b) {
        ip = addr;
    } else {
        ++ip;
    }
    return ip;
}

/**
 * if equals, set stack top to true
 * To be used often with jmps
 */
void exec_ieq(Stack *stack) {
    stack_obj_t b = stack_pop(stack);
    stack_obj_t a = stack_pop(stack);

    stack_obj_t is_equal;
    is_equal.type = BOOL;
    if (b.type != a.type) {
        is_equal.val.b = false;
    }
    switch (b.type) {
        case INT:
            is_equal.val.b = b.val.n == a.val.n;
            break;
        case BOOL:
            is_equal.val.b = b.val.b == a.val.b;
            break;
        case CHAR:
            is_equal.val.b = b.val.s == a.val.s;
            break;
        case FLOAT:
            is_equal.val.b = b.val.f == a.val.f;
            break;
        default:
            is_equal.val.b = false;
    }

    stack_push(stack, is_equal);
}

void exec_ilt(Stack *stack) {
    stack_obj_t b = stack_pop(stack);
    stack_obj_t a = stack_pop(stack);

    stack_obj_t is_less_than;
    is_less_than.type = BOOL;
    if (b.type != a.type) {
        is_less_than.val.b = false;
    }
    switch (b.type) {
        case INT:
            is_less_than.val.b = a.val.n < b.val.n;
            break;
        case BOOL:
            is_less_than.val.b = a.val.b < b.val.b;
            break;
        case CHAR:
            is_less_than.val.b = a.val.s < b.val.s;
            break;
        case FLOAT:
            is_less_than.val.b = a.val.f < b.val.f;
            break;
        default:
            is_less_than.val.b = false;
    }

    stack_push(stack, is_less_than);
}

void exec_pop(Stack *stack) {
    stack_pop(stack);
}

short exec_call(Stack *stack, Code *code, short ip, Memory *mem, const Function *fn_pool, short *caller_index) {
    short target_index = code_fetch(code, ++ip).val.addr;

    Function curr_fn = fn_pool[*caller_index];

    Function target_fn = fn_pool[target_index];

    std::cout << "HEREEE" << curr_fn.addr;

    short fp_new = mem->frame_ptr + curr_fn.locals;

    for (int i = 1; i <= target_fn.n_args; i++) {
        stack_obj_t v = stack_pop(stack);
        mem->locals[fp_new + target_fn.n_args - i] = v;
    }

    if (curr_fn.func_type == fn_t::NATIVE) {
        DLLoader * loader = get_dl_loader(curr_fn.lib_path);
        loader->DLOpenLib();
        void (*native_func)(...);
        native_func = reinterpret_cast<void (*)(...)> (loader->DLGetInstance(curr_fn.call_symbol.c_str()));
        native_func("Hello");
        loader->DLCloseLib();
        delete loader;
    }

    stack_obj_t ptr;
    ptr.type = ADDR;

    ptr.val.addr = *caller_index;
    stack_push(stack, ptr); // Store this to access it later

    ptr.val.addr = mem->frame_ptr;
    stack_push(stack, ptr); // current frame ptr

    ptr.val.addr = ip + (short) 1;
    stack_push(stack, ptr); // current ip

    mem->frame_ptr = fp_new;
    ip = fn_pool[target_index].addr;
    *caller_index = target_index;
    return ip;
}

short exec_ret(Stack *stack, Code *code, short ip, Memory *mem, const Function *fn_pool, short *caller_index) {
    stack_obj_t ret_val;
    int has_returned = fn_pool[*caller_index].return_type > 0;

    if (has_returned) {
        /* Is not void */
        ret_val = stack_pop(stack);
    }

    /* Restore the previous state from the stack */
    short ret_ip = stack_pop(stack).val.addr;
    mem->frame_ptr = stack_pop(stack).val.addr;
    *caller_index = stack_pop(stack).val.addr;

    if (has_returned) {
        stack_push(stack, ret_val);
    }

    return ret_ip;
}

void opcode_runner_init(Opcode *ops) {
    ops[NOP].type = NONE;
    ops[NOP].exec_none = exec_nop;

    ops[IADD].type = NOARGS;
    ops[IADD].exec_noargs = exec_iadd;

    ops[ISUB].type = NOARGS;
    ops[ISUB].exec_noargs = exec_isub;

    ops[IMUL].type = NOARGS;
    ops[IMUL].exec_noargs = exec_imul;

    ops[IDIV].type = NOARGS;
    ops[IDIV].exec_noargs = exec_idiv;

    ops[ICONST].type = WITH_ARGS;
    ops[ICONST].exec_args = exec_iconst;

    ops[PRINT].type = NOARGS;
    ops[PRINT].exec_noargs = exec_print;

    ops[PRINTLN].type = NOARGS;
    ops[PRINTLN].exec_noargs = exec_println;

    ops[LOAD].type = MEMORY_HANDLER;
    ops[LOAD].exec_mem = exec_load;

    ops[STORE].type = MEMORY_HANDLER;
    ops[STORE].exec_mem = exec_store;

    ops[JMP].type = UNCONDITIONAL_BRANCH;
    ops[JMP].exec_ujmp = exec_jmp;

    ops[JMPT].type = CONDITIONAL_BRANCH;
    ops[JMPT].exec_args = exec_jmpt;

    ops[ILT].type = NOARGS;
    ops[ILT].exec_noargs = exec_ilt;

    ops[IEQ].type = NOARGS;
    ops[IEQ].exec_noargs = exec_ieq;

    ops[POP].type = NOARGS;
    ops[POP].exec_noargs = exec_pop;

    ops[CALL].type = CALLER;
    ops[CALL].exec_caller = exec_call;

    ops[RET].type = CALLER;
    ops[RET].exec_caller = exec_ret;
}

