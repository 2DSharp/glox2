//
// Created by dedip on 08-10-2022.
//

#include "../include/code.h"

Code::Code(Bytecode *contents) {
    this->contents = contents;
}

Bytecode Code::code_fetch(int index) {
    return contents[index];
}

void Code::copy_contents(Code * code) {
    Code copy_code = *code;
    this->contents = copy_code.contents;
}

