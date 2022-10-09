//
// Created by dedip on 09-10-2022.
//
#include <iostream>
#include <dlfcn.h>
#include "../../include/dl_loader.h"

class UnixDLLoader : public DLLoader {
private:
    void *_handle;
    std::string _pathToLib;
    std::string _allocClassSymbol;
    std::string _deleteClassSymbol;

public:
    UnixDLLoader(std::string const &pathToLib) :
            _handle(nullptr), _pathToLib(pathToLib)
    {
    }
    ~UnixDLLoader() = default;

    void DLOpenLib() {
        if (!(_handle = dlopen(_pathToLib.c_str(), RTLD_NOW | RTLD_LAZY))) {
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