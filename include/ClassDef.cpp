//
// Created by dedip on 28-10-2022.
//

#include "ClassDef.h"

int ClassDef::get_function_index(const std::string& fn_name) {
    if (!fn_table.count(fn_name)) {
        throw std::invalid_argument("Illegal call to undeclared function \"" + fn_name + "()\"");
    }
    return fn_table.at(fn_name);
}

int ClassDef::get_var_index(const std::string& var_name) {
    if (!var_table.count(var_name)) {
        throw std::invalid_argument("Undeclared member variable \"" + var_name + "\"");

    }
    return var_table.at(var_name);
}

size_t ClassDef::get_num_fields() {
    return 0;
}
