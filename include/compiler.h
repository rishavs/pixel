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

// #define i_TYPE ListOfTokens, Token // Container type (name, element type)

// typedef struct ParsingContext {
//     FILE *file;   
//     Token* prev2ndToken;
//     Token* prev1stToken;
//     Token* currToken;
//     Token* next1stToken;
//     Token* next2ndToken;
// } ParsingContext;

// typedef struct IntNode {
//     int value;
// } IntNode;

void compile_file (char* filepath);
List* read_file (List*, char**, char*);
// ListOfTokens lex_file (cstr);
// int parse_file (ListOfTokens);

#endif // PIXEL_COMPILER_H