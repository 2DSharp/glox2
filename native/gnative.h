//
// Created by dedip on 11-10-2022.
//

#ifndef GLOX_GNATIVE_H
#define GLOX_GNATIVE_H

#include <string>
#define OP 0
#define INT 1
#define CHAR 2
#define FLOAT 3
#define BOOL 4
#define ADDR 5
#define STR 6

typedef union {
    short op;
    short addr;
    int n;
    char s;
    float f;
    bool b;
} GNative_Data;
typedef struct obj_t
{
    unsigned int type;
    GNative_Data val;
} GNative_OBJ;

extern "C"
{
void _invoke_gnative_function(GNative_OBJ* parameters, std::string func);
}
#endif //GLOX_GNATIVE_H
