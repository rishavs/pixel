#include <stdio.h>
#include "pixel.h"

#ifndef PIXEL_COMPILER_H
#define PIXEL_COMPILER_H

typedef struct Token
{
    enum {
        TOKEN_ILLEGAL,
        TOKEN_WHITESPACE,
        TOKEN_EOF,

        TOKEN_ID,
        TOKEN_INT,
        TOKEN_DECIMAL,
    } kind;
    char*       value;
    size_t      pos;
    size_t      line;
} Token;

void compile_file (char* filepath);
List* read_file (List*, char**, char*);

#endif // PIXEL_COMPILER_H