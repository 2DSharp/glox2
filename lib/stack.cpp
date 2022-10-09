//
// Created by dedip on 08-10-2022.
//

#include <iostream>
#include "../include/stack.h"

int Stack::push(stack_obj_t val) {
    contents[++top] = val;
    return top;
}

Stack::Stack(int size) {
    top = -1;
    this->size = size;
    this->contents = new stack_obj_t[size]();
}

void Stack::debug_print() {
    std::cout << "[ ";
    for ( int i = top; i > -1; i--) {
        std::cout << contents[i].val.n;
    }
    std::cout << "]";
}

stack_obj_t Stack::pop() {
    return contents[top--];
}

Stack::~Stack() {
    delete[] contents;
}

stack_obj_t Stack::get_top() {
    return contents[top];
}
