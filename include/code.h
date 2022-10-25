//
// Created by dedip on 08-10-2022.
//

#ifndef GLOX_CODE_H
#define GLOX_CODE_H

#include <cstddef>
#include "bytecode.h"

class Code {
private:
    Bytecode * contents;
public:
    Code(Bytecode *contents);
    Bytecode code_fetch(int index);

    void copy_contents(Code * code);
};


#endif //GLOX_CODE_H
