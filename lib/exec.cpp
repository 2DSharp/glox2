#include <dl_loader.h>
#include <sstream>
#include <utility>
#include "../include/exec.h"
#include "../include/array_descriptor.h"
#include "../include/dl_loader_factory.h"
#include "../include/obj_descriptor.h"
#include "../native/gnative.h"

template<typename T>
struct Callback;

template<typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
    template<typename... Args>
    static Ret callback(Args... args) {
        return func(args...);
    }

    static std::function<Ret(Params...)> func;
};

template<typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

Exec::Exec(Stack *stack, Code *code, Memory *mem, Function *fn_pool, std::map<std::string, ClassDef *> *loaded_classes)
        :
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

stack_obj_t create_parray(size_t size, int type, Memory *mem) {
    GObject *obj = GObjectFactory::create_array_descriptor(size, type);
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
    GObject *arr_obj = mem->heap->get_object(arr_descriptor->get_address_from_index(index.val.n));
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
    GObject *arr_obj = GObjectFactory::create_primitive_object(&value);
    mem->heap->alloc(arr_descriptor->get_address_from_index(index.val.n), arr_obj);
    return ++ip;
}

int resolve_const(Memory *mem, short addr) {
    int res;
    std::istringstream in(mem->constant_pool[addr]->val);
    in >> res;
    return res;
}

short Exec::e_set(short ip) {
    stack_obj_t obj_ref = stack_pop(stack);
    stack_obj_t val_ref = stack_pop(stack);
    GObject * val;
    if (val_ref.type == ADDR) {
         val = dynamic_cast<GObject *>(mem->heap->get_object(val_ref.val.addr));
    } else {
        val = GObjectFactory::create_primitive_object(&val_ref);
        mem->heap->alloc(val);
    }
    auto obj = dynamic_cast<ObjectDescriptor *>(mem->heap->get_object(obj_ref.val.addr));
    stack_obj_t field_index = code_fetch(++ip);
    obj->set_field(obj->get_context()->get_var_index(mem->constant_pool[field_index.val.addr]->val), val);

    return ++ip;
}

short Exec::e_get(short ip) {
    stack_obj_t obj_ref = stack_pop(stack);
    auto obj = dynamic_cast<ObjectDescriptor *>(mem->heap->get_object(obj_ref.val.addr));
    stack_obj_t field_index = code_fetch(++ip);
    auto val = obj->get_field(obj->get_context()->get_var_index(mem->constant_pool[field_index.val.addr]->val));
    stack_push(stack, {ADDR, val->get_address()});

    return ++ip;
}

void Exec::instantiate_object(const std::string &class_name) {
    ClassDef *context = get_class_context(class_name);
    GObject *descriptor = GObjectFactory::create_object_descriptor(context);
    mem->heap->alloc(descriptor);
    stack_obj_t obj_ref = {.type = ADDR, .val = {.addr = descriptor->get_address()}};
    stack_push(stack, obj_ref);
}

short Exec::e_new(short ip) {
    stack_obj_t cpool_ref = code_fetch(++ip);
    std::string class_name = mem->constant_pool[cpool_ref.val.addr]->val;
    instantiate_object(class_name);
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

GClass Exec::native_get_class(const char *cls) {
    auto class_def = loaded_classes->at(cls);
    std::string name = class_def->get_name();

    GClass class_obj;
    class_obj.class_name = new char();
    strcpy(class_obj.class_name, name.c_str());
    return class_obj;
}


short Exec::call_fn(short target_index, short ip, short *caller_index, vm_run_callback callback) {
    Function caller_fn = fn_pool[*caller_index];
    Function target_fn = fn_pool[target_index];

    stack_obj_t local_mem[target_fn.n_args];
    for (int i = 0; i < target_fn.n_args; i++) {

        local_mem[i] = stack_pop(stack);
    }


    if (target_fn.func_type == Function::NATIVE) {
        // Make a copy of the local variables into a subarray
        process_native_call(ip, caller_index, callback, target_fn, local_mem);
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

void Exec::process_native_call(short ip, short *caller_index, vm_run_callback callback,
                               Function &target_fn, stack_obj_t *local_mem) {
    DLLoader *loader = get_dl_loader(target_fn.lib_path);
    loader->DLOpenLib();
    void *(*native_func)(stack_obj_t *, std::string);
    // void (*init_runtime)(GRuntime*(*)());
// init_runtime = reinterpret_cast<void (*)(void (*)())> (loader->DLGetInstance("_initialize_glox_runtime"));
    void (*init_runtime)(GRuntime);
    init_runtime = reinterpret_cast<void (*)(GRuntime)> (loader->DLGetInstance("_initialize_glox_runtime"));
    Callback<GClass(const char *)>::func = std::bind(&Exec::native_get_class, this, std::placeholders::_1);
    auto native_get_class = static_cast<GClass (*)(const char *)>(Callback<GClass(const char *)>::callback);

    Callback<GNativeObj (GNativeObj, const char *, GParamList)>::func = std::bind(&Exec::native_invoke, this, ip,
                                                                                  caller_index,
                                                                                  callback,
                                                                                  std::placeholders::_1,
                                                                                  std::placeholders::_2,
                                                                                  std::placeholders::_3);
    auto native_invoke = static_cast<GNativeObj (*)(GNativeObj , const char *, GParamList)>
    (Callback<GNativeObj (GNativeObj, const char *, GParamList)>::callback);

    Callback<GNativeObj (GClass, GParamList)>::func = std::bind(&Exec::native_instantiate_obj, this, ip,
                                                                 caller_index,
                                                                 callback,
                                                                 std::placeholders::_1,
                                                                 std::placeholders::_2);
    auto native_instantiate = static_cast<GNativeObj (*)(GClass, GParamList)>
    (Callback<GNativeObj (GClass, GParamList)>::callback);

    Callback<const char * (GNativeString)>::func = std::bind(&Exec::native_get_str_chars, this,
                                                                std::placeholders::_1);
    auto native_get_str_chars = static_cast<const char * (*)(GNativeString)>
    (Callback<const char * (GNativeString)>::callback);


    GRuntime runtime = {
            .get_class = native_get_class,
            .invoke = native_invoke,
            .init_new = native_instantiate,
            .get_str_chars = native_get_str_chars
    };
    init_runtime(runtime);
    native_func = reinterpret_cast<void *(*)(stack_obj_t *, std::string)> (loader->DLGetInstance(
            "_invoke_gnative_function"));
    native_func(local_mem, target_fn.call_symbol);
    loader->DLCloseLib();
}

short Exec::e_call(short ip, short *caller_index, vm_run_callback runner) {
    short target_index = code_fetch(++ip).val.addr;
    return call_fn(target_index, ip, caller_index, runner);
}

short Exec::ocall_lib(const char *library_func, short ip, short *caller_index, const vm_run_callback& callback) {
    stack_obj_t obj_ref = stack_pop(stack);
    auto *obj = dynamic_cast<ObjectDescriptor *>(mem->heap->get_object(obj_ref.val.addr));
    short target_index = obj->get_context()->get_function_index(library_func);
    Function fn = fn_pool[target_index];
    auto current_context = fn_pool[*caller_index].context;
    stack_push(stack, obj_ref);
    switch (fn.scope) {
        case Function::PRIVATE: {
            if (current_context != obj->get_context()) {
                // throw some exception
                std::cout << "Private function called from a wrong context";
            } else {
                std::cout << "Private func called from the right context";
            }
            return call_fn(target_index, ip, caller_index, callback);
        }
        case Function::PROTECTED:
        case Function::PUBIC:
            ip = call_fn(target_index, ip, caller_index, callback);

    }
    return ip;
}

short Exec::e_ocall(short ip, short *caller_index, vm_run_callback callback) {

    short target_cpool_index = code_fetch(++ip).val.addr;
    auto func = mem->constant_pool[target_cpool_index]->val;
    return ocall_lib(func, ip, caller_index, callback);
}

/**
 * Loads data from the constant pool
 * @param stack
 * @param code
 * @param ip
 * @param mem
 * @return
 */
short Exec::e_cload(short ip, short *caller_index, vm_run_callback callback) {
    addr constant_ref = code_fetch(++ip).val.addr;
    Constant *constant_obj = mem->constant_pool[constant_ref];
    if (constant_obj->type == Constant::STRING) {
        // create an array and load it
        std::string str = constant_obj->val;
        stack_obj_t arr_index = create_parray(str.size(), CHAR, mem);
        auto *descriptor = dynamic_cast<ArrayDescriptor *> (mem->heap->get_object(arr_index.val.addr));
        for (int i = 0; i < str.size(); i++) {
            stack_obj_t obj = {.type = CHAR, .val = {.c = str.at(i)}};
            GObject *arr_obj = GObjectFactory::create_primitive_object(&obj);
            mem->heap->alloc(descriptor->get_address_from_index(i), arr_obj);
        }
        stack_push(stack, arr_index);
        instantiate_object("glox/core/type/String");
        return ocall_lib("init", ip, caller_index, std::move(callback));
//        find function index from symbol table
//         Call String library method to instantiate from an array of characters
        //return call_fn(stack, code, target_index, ip, fn_pool, caller_index);
    }
    return ++ip;
}

short Exec::e_ret(short ip, short *caller_index, vm_run_callback callback) {
    stack_obj_t ret_val;
    Function func = fn_pool[*caller_index];
    int has_returned = func.return_type > 0;

    if (has_returned) {
        /* Is not void */
        ret_val = stack_pop(stack);
    }
    if (func.func_type == Function::CTOR) {
        has_returned = 1;
        ret_val = stack->get_content(0);
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
    ops[SET] = {.type = WITH_ARGS, .exec_args = &Exec::e_set};
    ops[GET] = {.type = WITH_ARGS, .exec_args = &Exec::e_get};
    ops[NEW] = {.type = WITH_ARGS, .exec_args = &Exec::e_new};
    ops[CLOAD] = {.type = CALLER, .exec_caller = &Exec::e_cload};
    ops[OCALL] = {.type = CALLER, .exec_caller = &Exec::e_ocall};

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

ClassDef *Exec::get_class_context(const std::string &class_name) {
    return loaded_classes->at(class_name);
}

GNativeObj
Exec::native_invoke(short ip, short *caller_index, vm_run_callback callback, GNativeObj obj, const char *func,
                    GParamList params_meta) {
    auto params = params_meta.params;
    for (int i = 0; i < params_meta.size; i++) {
        stack->push(convert_native_to_stack_obj(params[i]));
    }
    stack->push(convert_native_to_stack_obj(obj));
    ip = ocall_lib(func, ip, caller_index, callback);
    callback(caller_index, ip);

    stack_obj_t res = stack->pop();

    return convert_stack_obj_to_native(res);
}

GNativeObj Exec::native_instantiate_obj(short ip, short *caller_index, vm_run_callback runner, GClass cls, GParamList params) {
    short * original_caller = caller_index;
    for (int i = 0; i < params.size; i++) {
        stack->push(convert_native_to_stack_obj(params.params[i]));
    }
    instantiate_object(cls.class_name);
    ip = ocall_lib("init", ip, caller_index, runner);
    caller_index = original_caller;
    runner(caller_index, ip);
    auto res = convert_stack_obj_to_native(stack->pop());
    return res;
}

const char * Exec::native_get_str_chars(GNativeString str) {
    // create a process to allow freeing of strings
    auto str_obj_ref = convert_native_to_stack_obj(str);
    if (str.type != ADDR) {
        switch (str.type) {
            case CHAR: {
                return new char[1]{str.val.c};
            }
            case INT: {
                auto res_str = std::to_string(str.val.n);
                char *char_arr = new char[res_str.size()];
                strcpy(char_arr, res_str.c_str());
                return char_arr;
            }
            default:
                return "garbage";
        }
    }
    auto str_obj = dynamic_cast<ObjectDescriptor *>(mem->heap->get_object(str_obj_ref.val.addr));
    auto str_arr = (ArrayDescriptor *) str_obj->get_field(str_obj->get_context()->get_var_index("value"));
    size_t len = str_arr->_size;
    char * res_str = new char[len + 1];
    for (int i = 0; i < len; i++) {
        res_str[i] = mem->heap->get_object(str_arr->get_address_from_index(i))->_data.val.c;
    }
    // Making a C-style char array string requires the buffer to be terminated with a null termination
    // https://stackoverflow.com/questions/20268769/why-do-i-get-garbage-value-when-output-character-array-to-console
    res_str[len] = '\0';
    return res_str;
}
