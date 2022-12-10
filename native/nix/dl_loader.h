//
// Created by dedip on 09-10-2022.
//

#ifndef GLOX_UNIX_DL_LOADER_H
#define GLOX_UNIX_DL_LOADER_H

#include <iostream>
#include <utility>
#include <dlfcn.h>
#include "../../include/dl_loader.h"

class UnixDLLoader : public DLLoader {
private:
    void *_handle;
    std::string _pathToLib;
    std::string _allocClassSymbol;
    std::string _deleteClassSymbol;

public:
    UnixDLLoader(std::string pathToLib) :
            _handle(nullptr), _pathToLib(std::move(pathToLib))
    {
    }
    ~UnixDLLoader() = default;

    void DLOpenLib() {
        if (!(_handle = dlopen(_pathToLib.c_str(), RTLD_NOW | RTLD_LAZY))) {
            std::cerr << dlerror() << std::endl;
        }
        if (_handle == nullptr) {
            std::cerr << dlerror() << std::endl;
        }

    }

    void DLCloseLib() override {
        if (dlclose(_handle) != 0) {
            std::cerr << dlerror() << std::endl;
        }
    }

    void* DLGetInstance(const char* symbol_str) override {
        void *func = nullptr;
        func = dlsym(_handle, symbol_str);

        if (nullptr == func)
        {
            std::cerr << dlerror() << std::endl;
        }

        return func;
    }
};
#endif // GLOX_UNIX_DL_LOADER_H