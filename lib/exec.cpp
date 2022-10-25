#include <dl_loader.h>
#include "../include/exec.h"
#include "../include/array_descriptor.h"
#include "../include/dl_loader_factory.h"

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
    std::cout << "Hello";
    stack_obj_t v = stack_pop(stack);
    switch (v.type) {
        case INT:
            std::cout << v.val.n;
            break;
        case BOOL:
            std::cout << v.val.b;
            break;
        case CHAR:
            std::cout << v.val.c;
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
    stack_push(stack, stack->get_content(offset));
    return ++ip;
}


short exec_store(Stack *stack, Code *code, short ip, Memory *mem) {

    short offset = code_fetch(code, ++ip).val.addr;
    stack->set_content(offset, stack_pop(stack));
    return ++ip;
}

short exec_jmp(Code *code, short ip) {
    return (code_fetch(code, ++ip).val.n);
}
stack_obj_t create_parray(size_t size, int type, Memory * mem) {
    GObject * obj = GObjectFactory::create_array_descriptor(size, type);
    addr ref = mem->heap->allocate_contiguous_block(obj, size);
    stack_obj_t res = {.type = ADDR, .val = {.addr = ref}};
    return res;
}
short exec_newparray(Stack * stack, Code *code, short ip, Memory *mem) {
    stack_obj_t size = stack_pop(stack);
    stack_obj_t type = stack_pop(stack);
    stack_obj_t res = create_parray(size.val.n, type.val.n, mem);
    stack_push(stack, res);
    return ++ip;
}

short exec_paload(Stack *stack, Code *code, short ip, Memory *mem) {
    stack_obj_t arr_ref = stack_pop(stack);
    auto arr_descriptor = dynamic_cast<ArrayDescriptor *>(mem->heap->get_object(arr_ref.val.addr));
    stack_obj_t index = stack_pop(stack);
    GObject * arr_obj = mem->heap->get_object(arr_descriptor->get_address_from_index(index.val.n));
    stack_obj_t primitive = arr_obj->_data;
    stack_push(stack, primitive);
    return ++ip;
}

short exec_alen(Stack *stack, Code *code, short ip, Memory *mem) {
    stack_obj_t arr_ref = stack_pop(stack);
    auto arr_descriptor = dynamic_cast<ArrayDescriptor *>(mem->heap->get_object(arr_ref.val.addr));
    stack_obj_t len = {.type = INT, .val = {.n = static_cast<int>(arr_descriptor->_size)}};
    stack_push(stack, len);
    return ++ip;
}

short exec_pastore(Stack *stack, Code *code, short ip, Memory *mem) {
    stack_obj_t arr_ref = stack_pop(stack);
    auto arr_descriptor = dynamic_cast<ArrayDescriptor *>(mem->heap->get_object(arr_ref.val.addr));
    stack_obj_t index = stack_pop(stack);
    stack_obj_t value = stack_pop(stack);
    GObject * arr_obj = GObjectFactory::create_primitive_object(&value);
    mem->heap->alloc(arr_descriptor->get_address_from_index(index.val.n), arr_obj);
    return ++ip;
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
            is_equal.val.b = b.val.c == a.val.c;
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
            is_less_than.val.b = a.val.c < b.val.c;
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

/**
 * Loads data from the constant pool
 * @param stack
 * @param code
 * @param ip
 * @param mem
 * @return
 */
short exec_cload(Stack *stack, Code *code, short ip, Memory *mem) {
    addr constant_ref = code_fetch(code, ++ip).val.addr;
    Constant * constant_obj = mem->constant_pool[constant_ref];
    if (constant_obj->type == Constant::STRING) {
        // create an array and load it
        std::string str = (const char *) constant_obj;
        stack_obj_t arr_index = create_parray(str.size(), CHAR, mem);
        auto * descriptor = dynamic_cast<ArrayDescriptor *> (mem->heap->get_object(arr_index.val.addr));
//        for (int i = 0; i < str.size(); i++) {
//            auto * obj = new stack_obj_t;
//            *obj = {.type = CHAR, .val.c = str.at(i)};
//            GObject * arr_obj = GObjectFactory::create_primitive_object(obj);
//            mem->heap->alloc(descriptor->get_address_from_index(i), arr_obj);
//            stack_push(stack, arr_index);
//            exec_call()
//            // Call String library method to instantiate from an array of characters
//        }
    }
    return ++ip;
}

short exec_call(Stack *stack, Code *code, short ip, Memory *mem, const Function *fn_pool, short *caller_index) {
    short target_index = code_fetch(code, ++ip).val.addr;
    Function caller_fn = fn_pool[*caller_index];
    Function target_fn = fn_pool[target_index];

    //short fp_new = stack->frame_ptr + caller_fn.n_args + caller_fn.locals + 3;

    stack_obj_t local_mem[target_fn.n_args];
    for (int i = 0; i < target_fn.n_args; i++) {

        local_mem[i] = stack_pop(stack);
    }


//    for (int i = 1; i <= target_fn.n_args; i++) {
//        stack_obj_t v = stack_pop(stack);
//        // Leave the fp, start from the next address otherwise it will overwrite whatever is in fp
//        stack->set_content(fp_new, target_fn.n_args - i, v);
//    }

    if (target_fn.func_type == Function::NATIVE) {
        // Make a copy of the local variables into a subarray

        DLLoader * loader = get_dl_loader(target_fn.lib_path);
        loader->DLOpenLib();
        void (*native_func)(stack_obj_t*,std::string);
        native_func = reinterpret_cast<void (*)(stack_obj_t*, std::string)> (loader->DLGetInstance("_invoke_gnative_function"));
        native_func(local_mem, target_fn.call_symbol);
        loader->DLCloseLib();

        return ++ip;
    }

    stack_obj_t ptr;
    ptr.type = ADDR;

    ptr.val.addr = *caller_index;
    stack_push(stack, ptr); // Store this to access it later

    ptr.val.addr = stack->frame_ptr;

    stack_push(stack, ptr); // current frame ptr

    ptr.val.addr = ip + 1;
    stack_push(stack, ptr); // The IP where it's going to continue after returning

    stack->frame_ptr = stack->top;

    for (int i = 0; i < target_fn.n_args; i++) {
        stack_push(stack, local_mem[i]);
    }
    for (int i = 0; i < target_fn.locals; i++) {
        stack->push({});
    }


    *caller_index = target_index; // Sets the new caller function index to the target function's index
    code->copy_contents(&target_fn.code);
    return 0; // call the 1st line of the target code
}

short exec_ret(Stack *stack, Code *code, short ip, Memory *mem, const Function *fn_pool, short *caller_index) {
    stack_obj_t ret_val;
    Function func = fn_pool[*caller_index];
    int has_returned = func.return_type > 0;

    if (has_returned) {
        /* Is not void */
        ret_val = stack_pop(stack);
    }

    stack->decrement_sp(func.n_args + func.locals);

    /* Restore the previous state from the stack */
    short ret_ip = stack_pop(stack).val.addr;
    stack->frame_ptr = stack_pop(stack).val.addr;
    *caller_index = stack_pop(stack).val.addr;

    if (has_returned) {
        stack_push(stack, ret_val);
    }
    auto caller_func = fn_pool[*caller_index];
    code->copy_contents(&caller_func.code);
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

    ops[NEW_PARRAY].type = MEMORY_HANDLER;
    ops[NEW_PARRAY].exec_mem = exec_newparray;

    ops[PALOAD] = {.type = MEMORY_HANDLER, .exec_mem = exec_paload};
    ops[PASTORE] = {.type = MEMORY_HANDLER, .exec_mem = exec_pastore};
    ops[ALEN] = {.type = MEMORY_HANDLER, .exec_mem = exec_alen};

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