//
// Created by dedip on 08-10-2022.
//

#include "vm.h"

VM::VM(size_t stack_size, Memory *mem) {
    this->stack = new Stack(stack_size);
    this->instr_ptr = 0;
    this->memory = mem;
    this->state = ST_HALTED;
}

void VM::vm_run(Code *code_mem, const Function *func_pool, short func_index, int debug) {
    short code;
    Opcode opcodes[128];

    opcode_runner_init(opcodes);
    this->instr_ptr = func_pool[func_index].addr;

    code = get_code(code_mem->code_fetch(this->instr_ptr));

    while (code != HALT && this->state != ST_INVALID) {
        /** Decode **/
        Opcode opcode = opcodes[code];

        if (debug) {
            //printf("\tTop: %d\n", this->stack->top);
            printf("IP: %03d\tOpcode: %04d\tFP: %02d ", this->instr_ptr, code, this->memory->frame_ptr);
            // print the local memory
            printf(" Mem: [ ");
            for (int i = 0; i < 10; i++) {
                printf("%d ", this->memory->locals[i].val.n);
            }
            printf(" ] ");
            printf(" Heap: [ ");

            for (int i = 0; i < 10; i++) {
                GObject * heap_obj = this->memory->heap->get_object(i);
                if (heap_obj != nullptr && heap_obj->_type == GObject::PRIMITIVE) {
                    printf("{T: %d D: %d I: %d} ",heap_obj->_data.type, heap_obj->_data.val.n, i);
                }
                if (heap_obj != nullptr && heap_obj->_type == GObject::ARRAY) {
                    auto arr = dynamic_cast<ArrayDescriptor *>(heap_obj);

                    std::cout << "{ P: " << arr->_size << " } ";
                }
            }
            printf(" ] ");
            //printf("fu: %d",func_pool[func_index].n_args);
        }
        /** Execute */
        vm_exec(code_mem, &opcode, func_pool, &func_index);
        code = get_code(code_mem->code_fetch(this->instr_ptr));

        if (debug) {
            stack->debug_print();
            printf("\tSP: %d\n", stack->get_top());
        }
    }
}

void VM::vm_close() {
    delete stack;
}

void VM::vm_exec(Code *code_mem, const Opcode *opcode, const Function *func_pool, short *func_index) {
    this->state = ST_RUNNING;
    switch (opcode->type) {
        case NOARGS:
            opcode->exec_noargs(this->stack);
            ++this->instr_ptr;
            break;
        case WITH_ARGS:
        case CONDITIONAL_BRANCH:
            this->instr_ptr = opcode->exec_args(this->stack, code_mem, this->instr_ptr);
            break;
        case UNCONDITIONAL_BRANCH:
            this->instr_ptr = opcode->exec_ujmp(code_mem, this->instr_ptr);
            break;
        case MEMORY_HANDLER:

            this->instr_ptr = opcode->exec_mem(this->stack, code_mem, this->instr_ptr, this->memory);
            break;

        case CALLER:
            this->instr_ptr = opcode->exec_caller(this->stack, code_mem, this->instr_ptr, this->memory, func_pool, func_index);
            break;

        case NONE:
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
