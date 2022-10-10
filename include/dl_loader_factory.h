//
// Created by dedip on 09-10-2022.
//

#ifndef GLOX_DL_LOADER_FACTORY_H
#define GLOX_DL_LOADER_FACTORY_H

#include "dl_loader.h"
#include "../native/nix/dl_loader.h"

DLLoader *get_dl_loader(const std::string lib_path) {
#ifdef __linux__
    return new UnixDLLoader(lib_path);
#endif

}

#endif // GLOX_DL_LOADER_FACTORY_H