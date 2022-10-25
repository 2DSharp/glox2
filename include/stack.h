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
    int size;
    stack_obj_t * contents;

public:
    int top;

    int frame_ptr = -1;

    int push(stack_obj_t val);
    Stack(int size);
    void debug_print();
    stack_obj_t pop();
    stack_obj_t get_top();
    void decrement_sp(int n);

    int get_stack_index(int fp, int index) {
        return fp + 1 + index;
    }

    void set_content(int offset, stack_obj_t obj) {
        int index = get_stack_index(frame_ptr, offset);
        contents[index] = obj;
        if (index > top) {
            top = index;
        }
    }
    void set_content(int new_frame_ptr, int offset, stack_obj_t obj) {
        int index = get_stack_index(new_frame_ptr, offset);
        contents[index] = obj;
        if (index > top) {
            top = index;
        }
    }
    stack_obj_t get_content(int index) {
        return contents[get_stack_index(frame_ptr, index)];
    }

    stack_obj_t get_content(int new_frame_ptr, int index) {
        return contents[get_stack_index(new_frame_ptr, index)];
    }
    ~Stack();

};



#endif //GLOX_STACK_H
