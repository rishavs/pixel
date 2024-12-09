#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "transpiler.h"

void create_Transpiler_context_t(Transpiler_context_t* ctx, const char* filepath) {
    // Initialize cursor
    ctx->i = 0;

    ctx->filepath = filepath;
    if (ctx->filepath == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    ctx->reading_duration = 0.0;

    // Initialize error handling
    ctx->errors_count = 0;
    ctx->errors_capacity = 8;
    ctx->errors = calloc(ctx->errors_capacity, sizeof(Transpiler_error_t));
    if (ctx->errors == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    
    // Initialize lexer
    ctx->tokens_count = 0;
    ctx->tokens_capacity = 8;
    ctx->tokens = calloc(ctx->tokens_capacity, sizeof(Token_t));
    if (ctx->tokens == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    // Initialize parser
    ctx->nodes_count = 0;
    ctx->nodes_capacity = 8;
    ctx->nodes =  calloc(ctx->nodes_capacity, sizeof(Node_t));
    if (ctx->nodes == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    ctx->parsing_duration = 0.0;  

    // add default c code
    ctx->c_code = DEFAULT_CFILE_CODE;
    ctx->h_code = "";
}

void add_error_to_context(Transpiler_context_t* ctx, const char* category, const char* header, const char* msg, const char* filepath, size_t line, size_t pos, const char* transpiler_file, size_t transpiler_line) {
    if (ctx->errors_count >= ctx->errors_capacity) {
        ctx->errors_capacity *= 2;
        ctx->errors = realloc(ctx->errors, ctx->errors_capacity * sizeof(Transpiler_error_t));
        if (ctx->errors == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    }

    ctx->errors[ctx->errors_count].category = strdup(category);
    ctx->errors[ctx->errors_count].header = strdup(header);
    ctx->errors[ctx->errors_count].msg = strdup(msg);
    ctx->errors[ctx->errors_count].filepath = strdup(filepath);
    ctx->errors[ctx->errors_count].line = line;
    ctx->errors[ctx->errors_count].pos = pos;
    ctx->errors[ctx->errors_count].transpiler_file = strdup(transpiler_file);
    ctx->errors[ctx->errors_count].transpiler_line = transpiler_line;

    ctx->errors_count++;
}

void add_token_to_context(Transpiler_context_t* ctx, Token_kind kind, size_t pos, size_t len, size_t line) {
    if (ctx->tokens_count >= ctx->tokens_capacity) {
        ctx->tokens_capacity *= 2;
        ctx->tokens = realloc(ctx->tokens, ctx->tokens_capacity * sizeof(Token_t));
        if (ctx->tokens == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
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
        if (ctx->nodes == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    }

    ctx->nodes[ctx->nodes_count].kind = kind;
    ctx->nodes[ctx->nodes_count].pos = pos;
    ctx->nodes[ctx->nodes_count].line = line;
    ctx->nodes_count++;
    return ctx->nodes_count;
}

size_t add_reference_to_args (Transpiler_context_t* ctx, size_t node_index, size_t arg_index) {
    if (ctx->nodes[node_index].args_count >= ctx->nodes[node_index].args_capacity) {
        ctx->nodes[node_index].args_capacity *= 2;
        ctx->nodes[node_index].args = realloc(ctx->nodes[node_index].args, ctx->nodes[node_index].args_capacity * sizeof(size_t));
        if (ctx->nodes[node_index].args == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    }

    ctx->nodes[node_index].args[ctx->nodes[node_index].args_count] = arg_index;
    ctx->nodes[node_index].args_count++;
    return ctx->nodes[node_index].args_count;
}

size_t add_reference_to_children (Transpiler_context_t* ctx, size_t node_index, size_t child_index) {
    if (ctx->nodes[node_index].children_count >= ctx->nodes[node_index].children_capacity) {
        ctx->nodes[node_index].children_capacity *= 2;
        ctx->nodes[node_index].children = realloc(ctx->nodes[node_index].children, ctx->nodes[node_index].children_capacity * sizeof(size_t));
        if (ctx->nodes[node_index].children == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    }

    ctx->nodes[node_index].children[ctx->nodes[node_index].children_count] = child_index;
    ctx->nodes[node_index].children_count++;
    return ctx->nodes[node_index].children_count;
}