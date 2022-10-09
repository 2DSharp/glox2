//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_OPCODE_H
#define GLOX_OPCODE_H

typedef void (* op_none)(void);
typedef void (* op_noargs)(Stack * stack);
typedef short (* op_args)(Stack * stack, Code * code, short ip);
typedef short (* op_ujmp)(Code *, short ip);
typedef short (* op_mem)(Stack * stack, Code * code, short ip, Memory * mem);
typedef short (* op_caller)(Stack * stack, Code * code, short ip, Memory * mem, const Function * fn_pool, short * caller_index);

typedef enum op_type {
    NONE,
    NOARGS,
    WITH_ARGS,
    UNCONDITIONAL_BRANCH,
    CONDITIONAL_BRANCH,
    MEMORY_HANDLER,
    CALLER,
    INVALID
} OpType;

typedef struct opcode_t
{
    short opcode;
    OpType type;
    union {
        op_none exec_none;
        op_noargs exec_noargs;
        op_args exec_args;
        op_ujmp exec_ujmp;
        op_caller exec_caller;
        op_mem exec_mem;
    };
} Opcode;

#endif //GLOX_OPCODE_H
