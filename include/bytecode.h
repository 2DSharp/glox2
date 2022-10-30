//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_BYTECODE_H
#define GLOX_BYTECODE_H

#define NOP 0
#define ICONST_1 1
#define ICONST_2 2
#define ICONST_3 3
#define ICONST_4 4
#define ICONST_5 5
#define ICONST 6
#define IADD 7
#define ISUB 8
#define IMUL 9
#define ILT 10
#define IEQ 11
#define JMP 12
#define JMPT 13
#define JMPF 14
#define LOAD 15
#define GLOAD 16
#define STORE 17
#define GSTORE 18
#define PRINT 19
#define HALT 20
#define IDIV 21
#define PRINTLN 22
#define IGT 23
#define POP 24
#define CALL 25
#define RET 26
#define NEW_PARRAY 27
#define PALOAD 28
#define PASTORE 29
#define ALEN 30
#define CLOAD 31
#define SET 32
#define GET 33
#define NEW 34
#define OCALL 35

#define OP 0
#define INT 1
#define CHAR 2
#define FLOAT 3
#define BOOL 4
#define ADDR 5
#define STR 6

typedef union {
    short op;
    short addr;
    int n;
    char c;
    float f;
    bool b;
} Data;
typedef struct bytecode_t
{
    unsigned int type;
    Data val;
} Bytecode;

#endif //GLOX_BYTECODE_H
