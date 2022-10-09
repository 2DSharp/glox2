//
// Created by dedip on 08-10-2022.
//

#include "../include/code.h"

Code::Code(Bytecode *contents, size_t size) {
    this->contents = contents;
    this->size = size;
}

Bytecode Code::code_fetch(int index) {
    return contents[index];
}
