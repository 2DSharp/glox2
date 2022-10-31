#include <iostream>
#include "lib/vm.h"

#define F_MAIN 0
#define F_ADD 1
#define F_FIB 1
#define F_PRINT 2
#define F_ARR 3
#define STR_SETVAL 4
#define STR_GETVAL 5
#define STR_INIT 6
#define F_PRINT_STR 7
#define F_GET_STR_FROM_CHAR_ARR 8


int main() {
    uint32_t stack_size = 2000;
    auto * c = new Constant(Constant::STRING, "Hello");
    Constant * const_pool[] = {
            new Constant(Constant::STRING, "Hello"),
            new Constant(Constant::CLASS, "glox/core/type/String"),
            new Constant(Constant::FUNC, "init"),
            new Constant(Constant::VAR, "value"),
            new Constant(Constant::FUNC, "setValue"),
            new Constant(Constant::FUNC, "getValue"),
    };

    auto *mem = new Memory(const_pool,
                             1024,
                             2048);

    // Run classloader here - forEach class file, generate the symbol tables and push to the class lut
    std::map<std::string, ClassDef*> loaded_classes;
    std::map<std::string, int> fn_table;
    fn_table["setValue"] = STR_SETVAL;
    fn_table["getValue"] = STR_GETVAL;
    fn_table["init"] = STR_INIT;
    std::map<std::string, int> var_table;
    var_table["value"] = 0;
    auto *string_class = new ClassDef("String", "glox/core/type", fn_table, var_table);

    Bytecode set_char_arr_to_str_obj[] = {
            {OP, LOAD}, {ADDR, {.addr = 1}}, // array
            {OP, LOAD}, {ADDR, {.addr = 0}}, // object ref
            {OP, SET}, {ADDR, {.addr = 0x3}}, // field name from constant pool
            {OP, RET}
    };

    Bytecode get_char_arr_to_str_obj[] = {
            {OP, LOAD}, {ADDR, {.addr = 0}}, // array
            {OP, GET}, {ADDR, {.addr = 0x3}}, // field name from constant pool
            {OP, RET}
    };

    Code arr_setter_code(set_char_arr_to_str_obj);
    Function str_setVal = { .locals = 0, .n_args = 2, .scope = Function::PUBIC, .context = string_class, .return_type = 0, .code = arr_setter_code};
    Function str_getVal = { .locals = 0, .n_args = 1, .scope = Function::PUBIC, .context = string_class,  .return_type = 1, .code = Code(get_char_arr_to_str_obj)};

    loaded_classes["glox/core/type/String"] = string_class;

    Bytecode create_new_string_from_array[] = {
            {OP, LOAD}, {ADDR, {.addr = 0}}, // load the array
            {OP, NEW}, {ADDR, {.addr = 0x1}}, // address of String in constant pool
            {OP, OCALL}, {ADDR, {.addr = 0x2}}, // address of invocation method in constant pool - init()
            {OP, RET}
    };

    Bytecode ctor_string_from_arr[] = {
            {OP, LOAD}, {ADDR, {.addr = 1}}, // array
            {OP, LOAD}, {ADDR, {.addr = 0}}, // object ref
            {OP, OCALL}, {ADDR, {.addr = 0x4}}, // setValue
            {OP, RET}
    };

    Bytecode print_string[] = {
            {OP, LOAD}, {ADDR, {.addr = 0}},
            {OP, GET}, {ADDR, {.addr = 0x03}}, // value
            {OP, STORE}, {ADDR, {.addr = 0}},
            {OP, ALEN}, // Store length in variable
            {OP, STORE}, {ADDR, {.addr = 2}}, // len = arr.length
            {OP, ICONST}, {INT, {.n = 0}},
            {OP, STORE}, {ADDR, {.addr = 1}}, // i = 0

            {OP, LOAD}, {INT, {.addr = 1}}, // #13 i = 0, 1, 2...
            {OP, LOAD}, {ADDR, {.addr = 0}}, // arr
            {OP, PALOAD}, // load arr[i]
            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // print arr[i]

            {OP, LOAD}, {INT, {.addr = 1}},
            {OP, ICONST}, {INT, {.n = 1}},
            {OP, IADD}, // i + 1
            {OP, STORE}, {ADDR, {.addr = 1}}, // i = i + 1
            {OP, LOAD}, {ADDR, {.addr = 1}}, // i
            {OP, LOAD}, {ADDR, {.addr = 2}}, // arr.length
            {OP, ILT}, // i < arr.length
            {OP, JMPT}, {ADDR, {.addr = 13}}, // loop back to 13
            {OP, ICONST} , {INT, {.n = 99}}, // TODO remove this
            {OP, RET}
//            {OP, CLOAD}, {ADDR, {.addr = 0x0}}, // references the symbol pool at addr 0
    };

    Function f_print_string = {.locals = 3, .n_args = 1, .return_type = 0, .code = Code(print_string)}; // print(string)
    Function f_string_init_def = {.locals = 0, .n_args = 2, .scope = Function::PUBIC,  .context = string_class, .return_type = 0, .func_type = Function::CTOR, .code = Code(ctor_string_from_arr) }; // new String(arr) -> definition
    Function f_new_string = {.locals = 0, .n_args = 1, .return_type = 1, .code = Code(create_new_string_from_array)}; // str = new string()

    VM *vm = new VM(stack_size, mem, &loaded_classes);

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
    f_fib.locals = 0;
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


    Function func_pool[10];
    func_pool[F_MAIN] = f_main;
    func_pool[F_FIB] = f_fib;
    func_pool[F_PRINT] = f_print;
    func_pool[STR_INIT] = f_string_init_def;
    func_pool[STR_SETVAL] = str_setVal;
    func_pool[STR_GETVAL] = str_getVal;
    func_pool[F_GET_STR_FROM_CHAR_ARR] = f_new_string;
    func_pool[F_PRINT_STR] = f_print_string;


    //printf("IP: %d\n", vm->instr_ptr);
   // vm->vm_run(func_pool, F_MAIN, 0);

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
//            {OP, LOAD}, {ADDR, {.addr = 0}},
//            {OP, ALEN}, // Store length in variable
//            {OP, STORE}, {ADDR, {.addr = 2}}, // len = arr.length
//            {OP, ICONST}, {INT, {.n = 0}},
//            {OP, STORE}, {ADDR, {.addr = 1}}, // i = 0
//            {OP, LOAD}, {ADDR, {.addr = 1}},
//            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // 69
//
//            {OP, LOAD}, {INT, {.addr = 1}},
//            {OP, LOAD}, {ADDR, {.addr = 0}},
//            {OP, PALOAD},
//            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // print arr[i]
//
//            {OP, LOAD}, {INT, {.addr = 1}},
//            {OP, ICONST}, {INT, {.n = 1}},
//            {OP, IADD},
//            {OP, STORE}, {ADDR, {.addr = 1}}, // i = i + 1
//            {OP, LOAD}, {ADDR, {.addr = 1}}, // i
//            {OP, LOAD}, {ADDR, {.addr = 2}}, // arr.length
//            {OP, ILT}, // i < arr.length
//            {OP, JMPT}, {ADDR, {.addr = 71}},
//            {OP, CLOAD}, {ADDR, {.addr = 0x0}}, // references the symbol pool at addr 0
//            {OP, CALL}, {ADDR, {.addr = F_PRINT}}, // print arr[i]
            {OP, LOAD}, {ADDR, {.addr = 0}},
            {OP, CALL}, {ADDR, {.addr = F_GET_STR_FROM_CHAR_ARR}},
            {OP, CALL}, {ADDR, {.addr = F_PRINT_STR}},
            {OP, HALT},
    };

    Code code_arr_main(arr_test);
    f_main.code = code_arr_main;
    func_pool[F_MAIN] = f_main;
    vm->vm_run(func_pool, F_MAIN, 0);
//
//    vm->vm_close();
//    delete code;
    delete mem;

    return 0;
}
