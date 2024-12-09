#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transpiler.h"

// Memory allocaltion failed. Fatal failure.
void memory_allocation_failure(char* transpiler_file, size_t transpiler_line) {
    fprintf(stderr, "\033[0;31m[ FATAL ] "); // Red color for FAILURE
    perror("Memory Allocation Failed!");
    fprintf(stderr, "Note: Your device might have run out of memory. Please close other programs and try again.\n\033[0m");
    fprintf(stderr, "Internal: Failure found in the compiler at line %zu in \"%s\"" , transpiler_line, transpiler_file);
    exit(EXIT_FAILURE);
}

// Unhandled error. Fatal failure.
void unhandled_error(const char* filepath, size_t at, size_t line, char* transpiler_file, size_t transpiler_line) {
    fprintf(stderr, "\033[0;31m[ FATAL ] "); // Red color for FAILURE
    fprintf(stderr, "Found an unhandled error at %zu on line %zu in %s\n", at, line, filepath);
    fprintf(stderr, "Note: This is a bug in the compiler! Please report with minimal reproducible code.\n\033[0m");
    fprintf(stderr, "Internal: Failure found in the compiler at line %zu in \"%s\"" , transpiler_line, transpiler_file);
    exit(EXIT_FAILURE);
}

// Helper - add error to context
void add_error_to_context(Transpiler_context_t* ctx, const char* category, const char* msg, const size_t pos, const size_t line, const char* transpiler_file, const size_t transpiler_line) {
    if (ctx->errors_count >= ctx->errors_capacity - 1) {
        ctx->errors_capacity *= 2;
        ctx->errors = realloc(ctx->errors, ctx->errors_capacity * sizeof(Transpiler_error_t));
        if (ctx->errors == NULL) memory_allocation_failure(__FILE__, __LINE__);
    }

    ctx->errors[ctx->errors_count].category = strdup(category);

    // TODO - handle very long file names? dynamically allocate memory?
    size_t msg_len = strlen(msg);
    char* error_pos = 
    sprintf("at %zu:%zu in the file \"%s\"\n", pos, line, ctx->filepath);
    size_t error_pos_len = strlen(error_pos);
    ctx->errors[ctx->errors_count].msg = calloc(msg_len + error_pos_len + 1, sizeof(char));
    if (ctx->errors[ctx->errors_count].msg == NULL) memory_allocation_failure(__FILE__, __LINE__);
    sprintf(ctx->errors[ctx->errors_count].msg, "%s %s", msg, error_pos);

    ctx->errors[ctx->errors_count].pos = pos;
    ctx->errors[ctx->errors_count].line = line;
    ctx->errors[ctx->errors_count].filepath = ctx->filepath;
    ctx->errors[ctx->errors_count].transpiler_file = transpiler_file;
    ctx->errors[ctx->errors_count].transpiler_line = transpiler_line;

    ctx->errors_count++;
}

// --------------------------------------
// Lexer Errors
// --------------------------------------
void illegal_char_error(Transpiler_context_t* ctx, const char c, const size_t pos, const size_t line, const char* transpiler_file, const size_t transpiler_line) {
    add_error_to_context(ctx, 
        "SyntaxError", 
        sprintf("Found illegal character \"%c\", ", c), 
        pos, line, transpiler_file, transpiler_line
    );
}

void unclosed_delimiter_error(Transpiler_context_t* ctx, char* delim, size_t pos, size_t line, char* transpiler_file, size_t transpiler_line) {
    add_error_to_context(ctx, 
        "SyntaxError", 
        sprintf("Found an unclosed delimiter \"%s\", ", delim),
        pos, line, transpiler_file, transpiler_line
    );
}

// --------------------------------------
// Parser Errors
// --------------------------------------
void missing_syntax_error(Transpiler_context_t* ctx, const char* expected_syntax, const size_t found_len, const size_t pos, size_t line, const char* transpiler_file, const size_t transpiler_line) {
    char* found = calloc (found_len + 1, sizeof(char));
    if (found == NULL) memory_allocation_failure(__FILE__, __LINE__);
    strncpy(found, ctx->src + pos, found_len);
    found[found_len] = '\0';

    add_error_to_context(ctx, 
        "SyntaxError", 
        sprintf("Expected %s, but instead found \"%s\", ", expected_syntax, found ), 
        pos, line, transpiler_file, transpiler_line
    );
}

void missing_token_error(Transpiler_context_t* ctx, const char* expected_token, const char* expected_syntax, const size_t found_len, const size_t pos, size_t line, const char* transpiler_file, const size_t transpiler_line) {
    char* found = calloc (found_len + 1, sizeof(char));
    if (found == NULL) memory_allocation_failure(__FILE__, __LINE__);
    strncpy(found, ctx->src + pos, found_len);
    found[found_len] = '\0';

    add_error_to_context(ctx, 
        "SyntaxError", 
        sprintf("Expected \"%s\" for %s, but instead found \"%s\", ", expected_token, expected_syntax, found ), 
        pos, line, transpiler_file, transpiler_line
    );
}

// --------------------------------------
// Symbol building Errors
// --------------------------------------
// void missing_symbol_initialization_error(const char* symbol, const char* filepath, size_t at, size_t line) {
//     fprintf(stderr, "\033[0;31m[ ERROR ] "); // Red color for Errors
//     fprintf(stderr, "Missing initialization for the symbol \"%s\" at %zu:%zu in %s\n", symbol, line, at, filepath);
// }
// void symbol_redeclaration_error(const char* symbol, const char* filepath, size_t at, size_t line) {
//     fprintf(stderr, "\033[0;31m[ ERROR ] "); // Red color for Errors
//     fprintf(stderr, "Previously declared Symbol \"%s\" is being redeclared at %zu:%zu in %s\n", symbol, line, at, filepath);
// }
