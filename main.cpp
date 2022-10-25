#include <iostream>
#include "lib/vm.h"

#define F_MAIN 0
#define F_ADD 1
#define F_FIB 1
#define F_PRINT 2
#define F_ARR 3
int main() {
    uint32_t stack_size = 2000;
    auto * c = new Constant(Constant::STRING, (void *) "Hello");
    Constant * const_pool[] = {
            c
    };

    Memory *mem = new Memory(const_pool,
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
    // printf("String %c", (char *)fib2[i].val);
    //}


      Bytecode fib_main[] = {
      // int fib_main(n)
    		 {OP, ICONST}, {INT, {.n = 10}},
             {OP, CALL}, {ADDR, {.addr = F_FIB}},
             {OP, CALL}, {ADDR, {.addr = F_PRINT}},
             {OP, HALT}
    };

      Bytecode  fib[] = {
              {OP, LOAD}, {ADDR, {.addr = 0}},
              {OP, ICONST}, {INT, {.n = 2}},
              {OP, ILT}, // if (n < 2) / n <= 1 goto 10 // 4
              {OP, JMPT}, {ADDR, {.addr = 23}}, // 5
              {OP, LOAD}, {ADDR, {.addr = 0}}, // n 7
              {OP, ICONST}, {INT, {.addr = 1}}, // 9
              {OP, ISUB}, // n - 1 // 11
              {OP, CALL}, {ADDR, {.addr = F_FIB}}, // fib(n - 1) 12
              {OP, LOAD}, {ADDR, {.addr = 0}}, // n 14
              {OP, ICONST}, {INT, {.n = 2}}, // 21
              {OP, ISUB}, // n - 23
              {OP, CALL}, {ADDR, {.addr = F_FIB}}, // fib(n-2) // 24
              {OP, IADD}, // fib(n-1) + fib(n-2) Line: 23 // 26
              {OP, RET}, // 22
              {OP, LOAD}, {ADDR, {.addr = 0}}, // else return n
              {OP, RET}
      };

    Code code_fib_main(fib_main);
    Code code_fib(fib);

    FILE *out;

    out = fopen("fib_main.glxc", "wb");
    if (out == nullptr) {
        return 1;
    }

  //  fwrite(code, sizeof(Code), 1, out);

    Function f_main;
//    f_main.addr = 0;
    f_main.locals = 3;
    f_main.n_args = 0;
    f_main.return_type = 0;
    f_main.code = code_fib_main;
    /*
    Function add;
    add.addr = 0;
    add.locals = 3;
    add.n_args = 2;
    add.return_type = 1;
    */
    Function f_fib;
//    f_fib.addr = 7;
    f_fib.locals = 1;
    f_fib.n_args = 1;
    f_fib.return_type = 1;
    f_fib.code = code_fib;

    Function f_print;
    f_print.locals = 1;
    f_print.n_args = 1;
    f_print.return_type = 1;
    f_print.func_type = Function::NATIVE;
    f_print.call_symbol = "GNative_io_print";
    f_print.lib_path = "native/libio.so";

    Function func_pool[4];
    func_pool[F_MAIN] = f_main;
    func_pool[F_FIB] = f_fib;
    func_pool[F_PRINT] = f_print;


    //printf("IP: %d\n", vm->instr_ptr);
    vm->vm_run(func_pool, F_MAIN, 1);

    //prcintf("IP: %d\n", vm->instr_ptr);
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
            // Now let'c iterate through the array
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
            {OP, CLOAD}, {ADDR, {.addr = 0}}, // references the symbol pool at addr 0
            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // print arr[i]
            {OP, HALT},
    };
//    code = new Code(arr_test);
//
//    vm->vm_run(code, func_pool, F_MAIN, 1);
//
//    vm->vm_close();
//    delete code;
    delete mem;

    return 0;
}
