cmake_minimum_required(VERSION 3.16)
project(glox)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    set(DLLOADER_SRC Core/DLLoader/Windows/DLLoader.h)
    include_directories(Core/DLLoader/Windows/)
endif(WIN32)

if(UNIX)
    set(DLLOADER_SRC native/nix/dl_loader.h)
    include_directories(native/nix)
endif(UNIX)
add_library(dl_loader ${DLLOADER_SRC})
set_target_properties(dl_loader PROPERTIES LINKER_LANGUAGE CXX)

add_executable(glox main.cpp lib/g_object_factory.cpp include/g_object_factory.h lib/g_object.cpp include/g_object.h lib/stack.cpp include/Heap.h lib/Heap.cpp include/stack.h lib/memory.cpp include/memory.h lib/vm.cpp lib/vm.h lib/code.cpp include/code.h include/bytecode.h include/function.h include/opcode.h include/exec.h lib/exec.cpp include/dl_loader.h include/dl_loader_factory.h)
target_link_libraries(glox dl_loader ${CMAKE_DL_LIBS})