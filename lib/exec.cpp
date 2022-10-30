#include <dl_loader.h>
#include <sstream>
#include <utility>
#include "../include/exec.h"
#include "../include/array_descriptor.h"
#include "../include/dl_loader_factory.h"
#include "../include/obj_descriptor.h"


Exec::Exec(Stack *stack, Code *code, Memory *mem, Function *fn_pool, std::map<std::string, ClassDef*> *loaded_classes) :
stack(stack), code(code), mem(mem), fn_pool(fn_pool), loaded_classes(loaded_classes) {
}
void Exec::e_nop() {
    return;
}

stack_obj_t stack_pop(Stack *stack) {
    return stack->pop();
}

Bytecode Exec::code_fetch(short index) {
    return code->code_fetch(index);
}

int stack_push(Stack *stack, stack_obj_t val) {
    return stack->push(val);
}

void Exec::e_iadd() {
    stack_obj_t b = stack_pop(stack);
    stack_obj_t a = stack_pop(stack);
    stack_obj_t res;
    res.type = INT;
    res.val.n = a.val.n + b.val.n;
    stack_push(stack, res);
}

void Exec::e_isub() {
    short b = stack_pop(stack).val.n;
    short a = stack_pop(stack).val.n;

    stack_obj_t res;
    res.type = INT;
    res.val.n = a - b;
    stack_push(stack, res);
}

void Exec::e_imul() {
    short b = stack_pop(stack).val.n;
    short a = stack_pop(stack).val.n;

    stack_obj_t res;
    res.type = INT;
    res.val.n = a * b;
    stack_push(stack, res);
}

void Exec::e_idiv() {
    short b = stack_pop(stack).val.n;
    short a = stack_pop(stack).val.n;

    stack_obj_t res;
    res.type = INT;
    res.val.n = a / b;

    stack_push(stack, res);
}

short Exec::e_iconst(short ip) {
    stack_obj_t v = code->code_fetch(++ip);
    stack_push(stack, v);
    return ++ip;
}

void Exec::e_print() {
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
void Exec::e_println() {
    //exec_print(stack);
    printf("\n");
}

short Exec::e_load(short ip) {
    short offset = code_fetch(++ip).val.addr;
    stack_push(stack, stack->get_content(offset));
    return ++ip;
}

short Exec::e_store(short ip) {

    short offset = code_fetch(++ip).val.addr;
    stack->set_content(offset, stack_pop(stack));
    return ++ip;
}

short Exec::e_jmp(short ip) {
    return (code_fetch(++ip).val.n);
}
stack_obj_t create_parray(size_t size, int type, Memory * mem) {
    GObject * obj = GObjectFactory::create_array_descriptor(size, type);
    addr ref = mem->heap->allocate_contiguous_block(obj, size);
    stack_obj_t res = {.type = ADDR, .val = {.addr = ref}};
    return res;
}
short Exec::e_newparray(short ip) {
    stack_obj_t size = stack_pop(stack);
    stack_obj_t type = stack_pop(stack);
    stack_obj_t res = create_parray(size.val.n, type.val.n, mem);
    stack_push(stack, res);
    return ++ip;
}

short Exec::e_paload(short ip) {
    stack_obj_t arr_ref = stack_pop(stack);
    auto arr_descriptor = dynamic_cast<ArrayDescriptor *>(mem->heap->get_object(arr_ref.val.addr));
    stack_obj_t index = stack_pop(stack);
    GObject * arr_obj = mem->heap->get_object(arr_descriptor->get_address_from_index(index.val.n));
    stack_obj_t primitive = arr_obj->_data;
    stack_push(stack, primitive);
    return ++ip;
}

short Exec::e_alen(short ip) {
    stack_obj_t arr_ref = stack_pop(stack);
    auto arr_descriptor = dynamic_cast<ArrayDescriptor *>(mem->heap->get_object(arr_ref.val.addr));
    stack_obj_t len = {.type = INT, .val = {.n = static_cast<int>(arr_descriptor->_size)}};
    stack_push(stack, len);
    return ++ip;
}

short Exec::e_pastore(short ip) {
    stack_obj_t arr_ref = stack_pop(stack);
    auto arr_descriptor = dynamic_cast<ArrayDescriptor *>(mem->heap->get_object(arr_ref.val.addr));
    stack_obj_t index = stack_pop(stack);
    stack_obj_t value = stack_pop(stack);
    GObject * arr_obj = GObjectFactory::create_primitive_object(&value);
    mem->heap->alloc(arr_descriptor->get_address_from_index(index.val.n), arr_obj);
    return ++ip;
}

int resolve_const(Memory * mem, short addr) {
    int res;
    std::istringstream in(mem->constant_pool[addr]->val);
    in >> res;
    return res;
}

short Exec::e_set(short ip) {
    stack_obj_t obj_ref = stack_pop(stack);
    stack_obj_t val_ref = stack_pop(stack);
    auto obj = dynamic_cast<ObjectDescriptor *>(mem->heap->get_object(obj_ref.val.addr));
    auto val = dynamic_cast<GObject *>(mem->heap->get_object(val_ref.val.addr));
    stack_obj_t field_index = code_fetch(++ip);
    obj->set_field(obj->get_context()->get_var_index(mem->constant_pool[field_index.val.addr]->val), val);

    return ++ip;
}

short Exec::e_new(short ip) {
    stack_obj_t cpool_ref = code_fetch(++ip);
    std::string class_name = mem->constant_pool[cpool_ref.val.addr]->val;
    ClassDef* context = get_class_context(class_name);
    GObject* descriptor = GObjectFactory::create_object_descriptor(context);
    addr address = mem->heap->alloc(descriptor);
    descriptor->set_address(address);
    return ++ip;
}

/*
 * Conditional Jump
 * If true on top of stack
 */
short Exec::e_jmpt(short ip) {
    short addr = code_fetch(++ip).val.addr;
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
void Exec::e_ieq() {
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

void Exec::e_ilt() {
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

void Exec::e_pop() {
    stack_pop(stack);
}

short Exec::call_fn(short target_index, short ip, short *caller_index) {
    Function caller_fn = fn_pool[*caller_index];
    Function target_fn = fn_pool[target_index];

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
short Exec::e_call(short ip, short *caller_index) {
    short target_index = code_fetch(++ip).val.addr;
    return call_fn(target_index, ip, caller_index);
}
short Exec::e_ocall(short ip, short *caller_index) {
    ObjectDescriptor * obj = dynamic_cast<ObjectDescriptor *>(mem->heap->get_object(stack_pop(stack).val.addr));
    short target_cpool_index = code_fetch(++ip).val.addr;
    short target_index = obj->get_context()->get_function_index(mem->constant_pool[target_cpool_index]->val);
    Function fn = fn_pool[target_index];
    auto current_context = fn_pool[*caller_index].context;
    switch (fn.scope) {
        case Function::PRIVATE: {
            if (current_context != obj->get_context()) {
                // throw some exception
            }
            return call_fn(target_index, ip, caller_index);
        }
        case Function::PROTECTED:
        case Function::PUBIC:
            return ip + 1;

    }
    return ip;
}
/**
 * Loads data from the constant pool
 * @param stack
 * @param code
 * @param ip
 * @param mem
 * @return
 */
short Exec::e_cload(short ip, short *caller_index) {
    addr constant_ref = code_fetch(++ip).val.addr;
    Constant * constant_obj = mem->constant_pool[constant_ref];
    if (constant_obj->type == Constant::STRING) {
         // create an array and load it
        std::string str = (const char *) constant_obj;
        stack_obj_t arr_index = create_parray(str.size(), CHAR, mem);
        auto * descriptor = dynamic_cast<ArrayDescriptor *> (mem->heap->get_object(arr_index.val.addr));
        for (int i = 0; i < str.size(); i++) {
            stack_obj_t obj = {.type = ADDR, .val = {.c = str.at(i)}};
            GObject * arr_obj = GObjectFactory::create_primitive_object(&obj);
            mem->heap->alloc(descriptor->get_address_from_index(i), arr_obj);
        }
        stack_push(stack, arr_index);

//        find function index from symbol table
//         Call String library method to instantiate from an array of characters
        //return call_fn(stack, code, target_index, ip, fn_pool, caller_index);
    }
    return ++ip;
}
short Exec::e_ret(short ip, short *caller_index) {
    stack_obj_t ret_val;
    Function func = fn_pool[*caller_index];
    int has_returned = func.return_type > 0;

    if (has_returned) {
        /* Is not void */
        ret_val = stack_pop(stack);
    }

    stack->decrement_sp(func.total_local_vars());

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

void Exec::opcode_runner_init(Opcode *ops) {
    ops[NOP].type = Exec::NONE;
    ops[NOP].exec_none = &Exec::e_nop;

    ops[IADD].type = NOARGS;
    ops[IADD].exec_noargs = &Exec::e_iadd;

 /*   Exec e;
    (e.*ops[IADD].exec_noargs)();*/

    ops[ISUB].type = NOARGS;
    ops[ISUB].exec_noargs = &Exec::e_isub;

    ops[IMUL].type = NOARGS;
    ops[IMUL].exec_noargs = &Exec::e_imul;

    ops[IDIV].type = NOARGS;
    ops[IDIV].exec_noargs = &Exec::e_idiv;

    ops[ICONST].type = WITH_ARGS;
    ops[ICONST].exec_args = &Exec::e_iconst;

    ops[PRINT].type = NOARGS;
    ops[PRINT].exec_noargs = &Exec::e_print;

    ops[PRINTLN].type = NOARGS;
    ops[PRINTLN].exec_noargs = &Exec::e_println;

    ops[LOAD].type = WITH_ARGS;
    ops[LOAD].exec_args = &Exec::e_load;

    ops[STORE].type = WITH_ARGS;
    ops[STORE].exec_args = &Exec::e_store;

    ops[NEW_PARRAY].type = WITH_ARGS;
    ops[NEW_PARRAY].exec_args = &Exec::e_newparray;

    ops[PALOAD] = {.type = WITH_ARGS, .exec_args = &Exec::e_paload};
    ops[PASTORE] = {.type = WITH_ARGS, .exec_args = &Exec::e_pastore};
    ops[ALEN] = {.type = WITH_ARGS, .exec_args = &Exec::e_alen};

    ops[JMP].type = WITH_ARGS;
    ops[JMP].exec_args = &Exec::e_jmp;

    ops[JMPT].type = WITH_ARGS;
    ops[JMPT].exec_args = &Exec::e_jmpt;

    ops[ILT].type = NOARGS;
    ops[ILT].exec_noargs = &Exec::e_ilt;

    ops[IEQ].type = NOARGS;
    ops[IEQ].exec_noargs = &Exec::e_ieq;

    ops[POP].type = NOARGS;
    ops[POP].exec_noargs = &Exec::e_pop;

    ops[CALL].type = CALLER;
    ops[CALL].exec_caller = &Exec::e_call;

    ops[RET].type = CALLER;
    ops[RET].exec_caller = &Exec::e_ret;
}

ClassDef *Exec::get_class_context(const std::string& class_name) {
    return loaded_classes->at(class_name);
}