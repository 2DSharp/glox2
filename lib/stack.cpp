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
    this->frame_ptr = -1;
}

void Stack::debug_print() {
    std::cout << "[ ";
    for ( int i = top; i > -1; i--) {
        switch (contents[i].type) {
            case ADDR:
                std::cout << contents[i].val.addr;
                break;
            case INT:
                std::cout << contents[i].val.n;
                break;
            case CHAR:
                std::cout << contents[i].val.c;
                break;
            case FLOAT:
                std::cout << contents[i].val.f;
                break;
            case OP:
                std::cout << contents[i].val.op;
                break;
            case BOOL:
                std::cout << contents[i].val.b;
                break;
            default:
                std::cout << "xx " << contents[i].val.n;
        }
        std::cout << " ";
    }
    std::cout << "]";
    printf("\tSP: %d\n", top);

}

stack_obj_t Stack::pop() {
    return contents[top--];
}

void Stack::decrement_sp(int n) {
    top = top - n;
}

Stack::~Stack() {
    delete[] contents;
}

stack_obj_t Stack::get_top() {
    return contents[top];
}
