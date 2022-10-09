//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_STACK_H
#define GLOX_STACK_H
#include <algorithm>
#include "bytecode.h"

typedef Bytecode stack_obj_t;

class Stack {
private:
    int top;
    int size;
    stack_obj_t * contents;

public:
    int push(stack_obj_t val);
    Stack(int size);
    void debug_print();
    stack_obj_t pop();
    stack_obj_t get_top();
    ~Stack();
};



#endif //GLOX_STACK_H
