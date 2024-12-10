#ifndef PIXEL_ERRORS_H
#define PIXEL_ERRORS_H

#include <stdlib.h>
#include <stdio.h>

#include "transpiler.h"

void memory_allocation_failure(size_t pos, size_t line, const char* filepath, char* transpiler_file, size_t transpiler_line);

void unhandled_error(size_t pos, size_t line, const char* filepath, char* transpiler_file, size_t transpiler_line);

void add_error_to_context(Transpiler_context_t* ctx, const char* category, const char* msg, const size_t pos, const size_t line, const char* transpiler_file, const size_t transpiler_line);

void illegal_char_error(Transpiler_context_t* ctx, const char c, const size_t pos, const size_t line, const char* transpiler_file, const size_t transpiler_line);

void unclosed_delimiter_error(Transpiler_context_t* ctx, char* delim, size_t pos, size_t line, char* transpiler_file, size_t transpiler_line);

void missing_syntax_error(Transpiler_context_t* ctx, const char* expected_syntax, const size_t found_len, const size_t pos, size_t line, const char* transpiler_file, const size_t transpiler_line);

void missing_token_error(Transpiler_context_t* ctx, const char* expected_token, const char* expected_syntax, const size_t found_len, const size_t pos, size_t line, const char* transpiler_file, const size_t transpiler_line);




#endif // PIXEL_ERRORS_H