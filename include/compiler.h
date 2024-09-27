#include <stdio.h>
#include "pixel.h"

#ifndef PIXEL_COMPILER_H
#define PIXEL_COMPILER_H

typedef struct CompilerError {
    char* category;
    char* header;
    char* msg;

    char* pixel_file;
    size_t pixel_line;
    size_t pixel_pos;

    char* c_file;
    size_t c_line;
} CompilerError;

void compiler_error_print(CompilerError* error);
void add_error_to_list(List* CompilerErrors, char* category, char* header, char* msg, char* pixel_file, size_t pixel_line, size_t pixel_pos, char* c_file, size_t c_line);

typedef struct Token
{
    // enum {
    //     TOKEN_ILLEGAL,
    //     TOKEN_WHITESPACE,
    //     TOKEN_EOF,

    //     TOKEN_ID,
    //     TOKEN_INT,
    //     TOKEN_DECIMAL,

    //     TOKEN_KWD_EXIT,

    //     TOKEN_LPAREN,
    //     TOKEN_RPAREN,
    // } kind;
    char*       kind;
    char*       value;
    size_t      pos;
    size_t      line;
} Token;

typedef struct Leaf Leaf;
typedef struct Leaf {
    // enum {
    //     PROGRAM_NODE,
    //     BLOCK_NODE,
    //     FUNC_NODE,
    //     VAR_NODE,
    // } kind;
    char* kind;
    size_t pos;
    size_t line;
    int variant;       // used for conveying any other attributes (eg. var vs const)
    char* value;       // for literals
    Leaf* parent;
    List* args;        // for args, params, and such
    List* statements;  // for blocks
} Leaf;


void compile_file (char* filepath);
bool read_file (List*, char**, char*);
bool lex_file (List* errors, List* tokens, char* src, char* filepath);
bool parse_file (List* errors, Leaf* program, List* tokens, char* filepath);

CompilerError* parse_integer(Leaf* int_node, List* tokens, size_t* i, char* filepath);
CompilerError* parse_expression(Leaf* expr_node, List* tokens, size_t* i, char* filepath);


#endif // PIXEL_COMPILER_H