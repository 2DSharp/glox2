//
// Created by dedip on 08-10-2022.
//

#include "vm.h"

VM::VM(size_t stack_size, Memory *mem, std::map<std::string, ClassDef *> *loaded_classes, Function *func_pool,
       int debug) {
    this->stack = new Stack(stack_size);
    this->instr_ptr = 0;
    this->memory = mem;
    this->state = ST_HALTED;
    this->loaded_classes = loaded_classes;
    this->func_pool = func_pool;
    this->debug = debug;
}

GNativeObj VM::vm_native_callback(short * func_index, short ip) {
    this->instr_ptr = ip;
    if (debug) {
        std::cout << std::endl;
    }
    vm_run(func_index, true); // till the function exits
}

void VM::vm_run(short func_index) {
    vm_run(&func_index, false);
}

void VM::vm_run(short * func_index, bool temp_spawn) {

    short code;
    Code *code_mem = new Code(nullptr);
    Function main = func_pool[*func_index];
    Code copy_main = main.code;
    code_mem->copy_contents(&copy_main);
    Exec::Opcode opcodes[128];
    for (int i = 0; i < main.locals; i++) {
        stack->push({});
    }

    Exec exec(stack, code_mem, memory, func_pool, loaded_classes);

    exec.opcode_runner_init(opcodes);

    code = get_code(code_mem->code_fetch(this->instr_ptr));

    while (code != HALT && this->state != ST_INVALID) {
        /** Decode **/
        Exec::Opcode opcode = opcodes[code];


        if (debug) {
            //printf("\tTop: %d\n", this->stack->top);
            std::cout << "F" << *func_index << " " << func_pool[*func_index].total_local_vars() << " ";
            printf("IP: %03d\tOpcode: %04d\tFP: %02d ", this->instr_ptr, code, this->stack->frame_ptr);
            // print the local memory
//            printf(" Mem: [ ");
//            for (int i = 0; i < 10; i++) {
//                printf("%d ", this->memory->locals[i].val.n);
//            }
//            printf(" ] ");
            printf(" Heap: [ ");

            for (int i = 0; i < 10; i++) {
                GObject *heap_obj = this->memory->heap->get_object(i);
                if (heap_obj != nullptr && heap_obj->_type == GObject::PRIMITIVE) {
                    printf("{T: %d D: %d I: %d} ", heap_obj->_data.type, heap_obj->_data.val.n, i);
                }
                if (heap_obj != nullptr && heap_obj->_type == GObject::ARRAY) {
                    auto arr = dynamic_cast<ArrayDescriptor *>(heap_obj);

                    std::cout << "{ A: " << arr->_size << " } ";
                }
                if (heap_obj != nullptr && heap_obj->_type == GObject::OBJ) {
                    auto obj = dynamic_cast<ObjectDescriptor *>(heap_obj);
                    std::cout << " { O: " << obj->get_context()->get_name() << "@" << obj->get_address() << " } ";
                }
            }
            printf(" ] ");
            //printf("fu: %d",func_pool[func_index].n_args);
            fflush(stdout);
        }
        /** Execute */
        vm_exec(exec, &opcode, func_index);

        if (debug) {
            stack->debug_print();
        }
        if (temp_spawn && code == RET) {
            return;
        }
        code = get_code(code_mem->code_fetch(this->instr_ptr));

    }
}

void VM::vm_close() {
    delete stack;
}

void VM::vm_exec(Exec exec, const Exec::Opcode *opcode, short *func_index) {
    this->state = ST_RUNNING;
    switch (opcode->type) {
        case Exec::NOARGS: {
            auto ptr = opcode->exec_noargs;
            (exec.*ptr)();
            ++this->instr_ptr;
            break;
        }
        case Exec::WITH_ARGS: {
            auto ptr = opcode->exec_args;
            instr_ptr = (exec.*ptr)(instr_ptr);
            break;
        }

        case Exec::CALLER: {
            auto ptr = opcode->exec_caller;
            instr_ptr = (exec.*ptr)(instr_ptr, func_index,
                                    std::bind(&VM::vm_native_callback, this, std::placeholders::_1,
                                              std::placeholders::_2));
            break;
        }
        case Exec::NONE:
            //opcode->exec_none();
            ++this->instr_ptr;
            break;

        default:
            this->state = ST_INVALID;
            return;
    }
}

short VM::get_code(Bytecode code) {
    return code.val.op;
}
