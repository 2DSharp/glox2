//
// Created by dedip on 28-10-2022.
//

#ifndef GLOX_CLASSDEF_H
#define GLOX_CLASSDEF_H


#include <string>
#include <map>
#include <utility>
#include "constants.h"

class ClassDef {
private:
    size_t _num_fields;
    std::string name;
    std::string name_space;
    std::map<std::string, int> fn_table;
    std::map<std::string, int> var_table;

public:
    ClassDef(std::string name, std::string nameSpace, std::map<std::string, int> fnTable,
             std::map<std::string, int> varTable) : name(std::move(name)), name_space(std::move(nameSpace)), fn_table(std::move(fnTable)),
                                                           var_table(std::move(varTable)) {
    }

    int get_function_index(const std::string &);

    int get_var_index(const std::string &var_name);

    size_t get_num_fields();

    std::string get_name() {
        return name_space + "/" + name;
    }
};

#endif //GLOX_CLASSDEF_H
