#include <iostream>
#include "lib/vm.h"

#define F_MAIN 0
#define F_ADD 1
#define F_FIB 1
#define F_PRINT 2
#define F_ARR 3
int main() {
    uint32_t stack_size = 2000;
    Memory *mem = new Memory(1024,
                             1024,
                             2048);

    VM *vm = new VM(stack_size, mem);

    /* short instrs[] = { ICONST, 0,
               STORE, 1,
               LOAD, 1,
               ICONST, 1,
               IADD,
               PRINT,
               STORE, 1,
               LOAD, 1,
               ICONST, 50,
               ILT,
               JMPT, 4,
               HALT };
    */

    /*
    short instrs[] = {LOAD, 0, //int add(x, y) {
              LOAD, 1,
              IADD,
              STORE, 2, // c = x + y
              LOAD, 2,   // return c
              RET,
              // void main() {
              ICONST, 10,
              STORE, 0,
              ICONST, 17,
              STORE, 1,
              LOAD, 0,
              LOAD, 1,
              CALL, 1, // add(10, 17)
              PRINTLN, // print add(10, 17)
              HALT };

    */

    Bytecode fib2[] = {
            {OP, {ICONST}},
            {INT, {.n = 20}},
            {OP,  {PRINTLN}},
            {OP, {HALT}}

    };


    // for (int i = 0; i < sizeof(fib2)/sizeof(Bytecode); i++) {
    // printf("String %s", (char *)fib2[i].val);
    //}


      Bytecode fib[] = {
      // int fib(n)
    		 {OP, ICONST}, {INT, {.n = 4}},
             {OP, CALL}, {ADDR, {.addr = F_FIB}},
             {OP, CALL}, {ADDR, {.addr = F_PRINT}},
             {OP, HALT},
             {OP, LOAD}, {ADDR, {.addr = 0}},
             {OP, ICONST}, {INT, {.n = 2}},
             {OP, ILT}, // if (n < 2) / n <= 1 goto 10
             {OP, JMPT}, {ADDR, {.addr = 30}},
             {OP, LOAD}, {ADDR, {.addr = 0}}, // n
             {OP, ICONST}, {INT, {.addr = 1}},
             {OP, ISUB}, // n - 1
             {OP, CALL}, {ADDR, {.addr = F_FIB}}, // fib(n - 1) 17
             {OP, LOAD}, {ADDR, {.addr = 0}}, // n 19
             {OP, ICONST}, {INT, {.n = 2}}, // 21
             {OP, ISUB}, // n - 23
             {OP, CALL}, {ADDR, {.addr = F_FIB}}, // fib(n-2) // 24
             {OP, IADD}, // fib(n-1) + fib(n-2) Line: 23 // 26
             {OP, RET}, // 27
             {OP, LOAD}, {ADDR, {.addr = 0}}, // else return n
             {OP, RET}
    };

    Code *code = new Code(fib, 2);

    FILE *out;

    out = fopen("fib.glxc", "wb");
    if (out == nullptr) {
        return 1;
    }

    fwrite(code, sizeof(Code), 1, out);

    Function f_main;
    f_main.addr = 0;
    f_main.locals = 3;
    f_main.n_args = 0;
    f_main.return_type = 0;
    /*
    Function add;
    add.addr = 0;
    add.locals = 3;
    add.n_args = 2;
    add.return_type = 1;
    */
    Function f_fib;
    f_fib.addr = 7;
    f_fib.locals = 1;
    f_fib.n_args = 1;
    f_fib.return_type = 1;

    Function f_print;
    f_print.locals = 1;
    f_print.n_args = 1;
    f_print.return_type = 1;
    f_print.func_type = fn_t::NATIVE;
    f_print.call_symbol = "GNative_io_print";
    f_print.lib_path = "native/libio.so";

    Function func_pool[4];
    func_pool[F_MAIN] = f_main;
    func_pool[F_FIB] = f_fib;
    func_pool[F_PRINT] = f_print;

    //printf("IP: %d\n", vm->instr_ptr);
    vm->vm_run(code, func_pool, F_MAIN, 0);

    //printf("IP: %d\n", vm->instr_ptr);
    Bytecode arr_test[] = {
            {OP, ICONST}, {INT, {.n = INT}},
            {OP, ICONST}, {INT, {.n = 4}},
            {OP, NEW_PARRAY}, // arr_ref is on top of stack
//            {OP, CALL}, {ADDR, {.addr = F_PRINT}},
            {OP, STORE}, {ADDR, {.addr = 0}},
            {OP, ICONST}, {INT, {.n = 33}}, // value
            {OP, ICONST}, {INT, {.n = 1}}, // index
            {OP, LOAD}, {ADDR, {.addr = 0}}, //  ref
            {OP, PASTORE},
            {OP, ICONST}, {INT, {.n = 1}},
            {OP, LOAD}, {ADDR, {.addr = 0}},
            {OP, PALOAD},
            {OP, CALL}, {ADDR, {.addr = F_PRINT}},
            {OP, LOAD}, {ADDR, {.addr = 0}},
            {OP, ALEN},
            {OP, CALL}, {ADDR, {.addr = F_PRINT}},
            {OP, ICONST}, {INT, {.n = 1000}},
            {OP, CALL}, {ADDR, {.addr = F_PRINT}},

            {OP, ICONST}, {INT, {.n = 75}}, // value
            {OP, ICONST}, {INT, {.n = 1}}, // index
            {OP, LOAD}, {ADDR, {.addr = 0}}, //  ref
            {OP, PASTORE},

            {OP, ICONST}, {INT, {.n = -109}}, // value
            {OP, ICONST}, {INT, {.n = 0}}, // index
            {OP, LOAD}, {ADDR, {.addr = 0}}, //  ref
            {OP, PASTORE},

            {OP, ICONST}, {INT, {.n = 9}}, // value
            {OP, ICONST}, {INT, {.n = 3}}, // index
            {OP, LOAD}, {ADDR, {.addr = 0}}, //  ref
            {OP, PASTORE},

            {OP, ICONST}, {INT, {.n = 777}}, // value
            {OP, ICONST}, {INT, {.n = 2}}, // index
            {OP, LOAD}, {ADDR, {.addr = 0}}, //  ref
            {OP, PASTORE},
            // --- Stored data here --
            // Now let's iterate through the array
            {OP, LOAD}, {ADDR, {.addr = 0}},
            {OP, ALEN}, // Store length in variable
            {OP, STORE}, {ADDR, {.addr = 2}}, // len = arr.length
            {OP, ICONST}, {INT, {.n = 0}},
            {OP, STORE}, {ADDR, {.addr = 1}}, // i = 0
            {OP, LOAD}, {ADDR, {.addr = 1}},
            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // 69

            {OP, LOAD}, {INT, {.addr = 1}},
            {OP, LOAD}, {ADDR, {.addr = 0}},
            {OP, PALOAD},
            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // print arr[i]

            {OP, LOAD}, {INT, {.addr = 1}},
            {OP, ICONST}, {INT, {.n = 1}},
            {OP, IADD},
            {OP, STORE}, {ADDR, {.addr = 1}}, // i = i + 1
            {OP, LOAD}, {ADDR, {.addr = 1}}, // i
            {OP, LOAD}, {ADDR, {.addr = 2}}, // arr.length
            {OP, ILT}, // i < arr.length
            {OP, JMPT}, {ADDR, {.addr = 71}},
            {OP, HALT},
    };
    code = new Code(arr_test, 2);

    vm->vm_run(code, func_pool, F_MAIN, 0);

    vm->vm_close();
    delete code;
    delete mem;

    return 0;
}
