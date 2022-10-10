//
// Created by dedip on 09-10-2022.
//

#ifndef GLOX_DL_LOADER_H
#define GLOX_DL_LOADER_H

#pragma once

#include <memory>
#include <string>

/*
** Interface for Dynamic Library Loading (DLLoader)
** API for Unix and Windows. Handling of open, close, validity-check.
*/
class DLLoader {

public:

    /*
    **
    */
    virtual void DLOpenLib() = 0;

    /*
    ** Return a shared pointer on an instance of class loaded through
    ** a dynamic library.
    */
    virtual void * DLGetInstance(const char* symbol_str) = 0;

    /*
    ** Correctly delete the instance of the "dynamically loaded" class.
    */
    virtual void DLCloseLib() = 0;

};


#endif //GLOX_DL_LOADER_H
