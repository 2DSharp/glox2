//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_CODE_H
#define GLOX_CODE_H

#include <cstddef>
#include "bytecode.h"

class Code {
private:
    size_t size;
    Bytecode * contents;
public:
    Code(Bytecode * contents, size_t size);
    Bytecode code_fetch(int index);
};


#endif //GLOX_CODE_H
