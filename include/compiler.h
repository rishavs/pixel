#include <stdio.h>
#include "pixel.h"

#ifndef PIXEL_COMPILER_H
#define PIXEL_COMPILER_H


#define COMPILER_ERROR_MAX_CATEGORY_LENGTH 64
#define COMPILER_ERROR_MAX_HEADER_LENGTH 256
#define COMPILER_ERROR_MAX_MSG_LENGTH 1024
#define COMPILER_ERROR_MAX_FILE_LENGTH 1024

typedef struct CompilerError {
    char    category[COMPILER_ERROR_MAX_CATEGORY_LENGTH];
    char    header[COMPILER_ERROR_MAX_HEADER_LENGTH];
    char    msg[COMPILER_ERROR_MAX_MSG_LENGTH];

    char    pixel_file[COMPILER_ERROR_MAX_FILE_LENGTH];
    size_t  pixel_line;
    size_t  pixel_pos;

    char    c_file[COMPILER_ERROR_MAX_FILE_LENGTH];
    size_t  c_line;
} CompilerError;
void compiler_error_print(CompilerError* error);
void add_error_to_list(List* CompilerErrors, const char* category, const char* header, const char* msg, const char* pixel_file, size_t pixel_line, size_t pixel_pos, const char* c_file, size_t c_line);

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
bool read_file (List*, char**, char*);
bool lex_file (List* errors, List* tokens, char* src, char* filepath);

#endif // PIXEL_COMPILER_H