#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "transpiler.h"

TRANSPILER_CONTEXT* create_transpiler_context(const char* filepath) {
    TRANSPILER_CONTEXT* ctx = (TRANSPILER_CONTEXT*)malloc(sizeof(TRANSPILER_CONTEXT));
    if (ctx == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    ctx->filepath = strdup(filepath);
    ctx->src = "";

    // Initialize error handling
    ctx->errors_count = 0;
    ctx->errors_capacity = 8; // Initial capacity for errors
    ctx->errors = calloc(ctx->errors_capacity, sizeof(TRANSPILER_ERROR));
    if (ctx->errors == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    // Initialize cursor
    ctx->i = 0;

    // Initialize lexer
    ctx->tokens_count = 0;
    ctx->tokens_capacity = 8; // Initial capacity for tokens
    ctx->tokens = calloc(ctx->tokens_capacity, sizeof(TOKEN));
    if (ctx->tokens == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    ctx->lexing_duration = 0.0;

    // add default c code
    ctx->cFileCode = "\
int main() {\
    return 0;\
}\
";
    ctx->hFileCode = "";

    return ctx;
}

void add_error_to_context(TRANSPILER_CONTEXT* ctx, char* category, char* header, char* msg, char* filepath, size_t pos, size_t line, char* transpiler_file, size_t transpiler_line) {
    if (ctx->errors_count == ctx->errors_capacity) {
        ctx->errors_capacity *= 2;
        ctx->errors = (TRANSPILER_ERROR*)realloc(ctx->errors, ctx->errors_capacity * sizeof(TRANSPILER_ERROR));
        if (ctx->errors == NULL) {
            fatal_memory_allocation_failure(__FILE__, __LINE__);
        }
    }

    ctx->errors[ctx->errors_count].category = strdup(category);
    ctx->errors[ctx->errors_count].header = strdup(header);
    ctx->errors[ctx->errors_count].msg = strdup(msg);
    ctx->errors[ctx->errors_count].filepath = strdup(filepath);
    ctx->errors[ctx->errors_count].pos = pos;
    ctx->errors[ctx->errors_count].line = line;
    ctx->errors[ctx->errors_count].transpiler_file = strdup(transpiler_file);
    ctx->errors[ctx->errors_count].transpiler_line = transpiler_line;
    ctx->errors_count++;
}

void add_token_to_context(TRANSPILER_CONTEXT* ctx, TOKENKIND kind, char* value, size_t pos, size_t line) {
    if (ctx->tokens_count == ctx->tokens_capacity) {
        ctx->tokens_capacity *= 2;
        ctx->tokens = (TOKEN*)realloc(ctx->tokens, ctx->tokens_capacity * sizeof(TOKEN));
        if (ctx->tokens == NULL) {
            fatal_memory_allocation_failure(__FILE__, __LINE__);
        }
    }

    ctx->tokens[ctx->tokens_count].kind = kind;
    ctx->tokens[ctx->tokens_count].value = strdup(value);
    ctx->tokens[ctx->tokens_count].pos = pos;
    ctx->tokens[ctx->tokens_count].line = line;
    ctx->tokens_count++;

}
