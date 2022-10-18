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
    int frame_ptr;

    int push(stack_obj_t val);
    Stack(int size);
    void debug_print();
    stack_obj_t pop();
    stack_obj_t get_top();
    void set_content(int index, stack_obj_t obj) {
        contents[frame_ptr + 1 + index] = obj;
    }
    void set_content(int new_frame_ptr, int index, stack_obj_t obj) {
        contents[new_frame_ptr + 1 + index] = obj;
    }
    stack_obj_t get_content(int index) {
        return contents[frame_ptr + 1 + index];
    }

    stack_obj_t get_content(int new_frame_ptr, int index) {
        return contents[new_frame_ptr + 1 + index];
    }
    ~Stack();
};



#endif //GLOX_STACK_H
