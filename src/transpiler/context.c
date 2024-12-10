#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "transpiler.h"

void create_Transpiler_context_t(Transpiler_context_t* ctx, const char* filepath) {
    // Initialize cursor
    ctx->i = 0;

    ctx->filepath = strdup(filepath);
    if (ctx->filepath == NULL) memory_allocation_failure(0, 0, filepath, __FILE__, __LINE__);

    ctx->reading_duration = 0.0;

    // Initialize error handling
    ctx->errors_count = 0;
    ctx->errors_capacity = 8;
    ctx->errors = calloc(ctx->errors_capacity, sizeof(Transpiler_error_t));
    if (ctx->errors == NULL) memory_allocation_failure(0, 0, filepath, __FILE__, __LINE__);

    
    // Initialize lexer
    ctx->tokens_count = 0;
    ctx->tokens_capacity = 8;
    ctx->tokens = calloc(ctx->tokens_capacity, sizeof(Token_t));
    if (ctx->tokens == NULL) memory_allocation_failure(0, 0, filepath, __FILE__, __LINE__);

    // Initialize parser
    ctx->nodes_count = 0;
    ctx->nodes_capacity = 8;
    ctx->nodes =  calloc(ctx->nodes_capacity, sizeof(Node_t));
    if (ctx->nodes == NULL) memory_allocation_failure(0, 0, filepath, __FILE__, __LINE__);
    ctx->parsing_duration = 0.0;  

    // add default c code
    ctx->c_code = DEFAULT_CFILE_CODE;
    ctx->h_code = "";
}

void add_token_to_context(Transpiler_context_t* ctx, Token_kind kind, size_t pos, size_t len, size_t line) {
    if (ctx->tokens_count >= ctx->tokens_capacity) {
        ctx->tokens_capacity *= 2;
        ctx->tokens = realloc(ctx->tokens, ctx->tokens_capacity * sizeof(Token_t));
        if (ctx->tokens == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);
    }

    ctx->tokens[ctx->tokens_count].kind = kind;
    ctx->tokens[ctx->tokens_count].pos = pos;
    ctx->tokens[ctx->tokens_count].len = len;
    ctx->tokens[ctx->tokens_count].line = line;

    ctx->tokens_count++;
}

size_t add_node_to_context(Transpiler_context_t* ctx, Node_kind kind, size_t pos, size_t line) {
    if (ctx->nodes_count >= ctx->nodes_capacity) {
        ctx->nodes_capacity *= 2;
        ctx->nodes = realloc(ctx->nodes, ctx->nodes_capacity * sizeof(Node_t));
        if (ctx->nodes == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);
    }

    size_t at = ctx->nodes_count;

    ctx->nodes[at].kind = kind;
    ctx->nodes[at].pos = pos;
    ctx->nodes[at].line = line;
    ctx->nodes_count++;
    return at;
}

size_t add_to_indices(size_t* indices_list, size_t index, size_t count, size_t capacity) {
    if (count >= capacity) {
        capacity *= 2;
        indices_list = realloc(indices_list, capacity * sizeof(size_t));
        if (indices_list == NULL) memory_allocation_failure(0, 0, NULL, __FILE__, __LINE__);
    }
    size_t at = count;
    indices_list[at] = index;
    count++;
    return at;
}