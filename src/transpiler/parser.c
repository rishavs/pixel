#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "helpers.h"
#include "resources.h"
#include "errors.h"
#include "transpiler.h"

bool parse_expression(Transpiler_context_t* ctx, size_t parent_index, size_t scope_owner_index);

bool parse_declaration(Transpiler_context_t* ctx, size_t parent_index, size_t scope_owner_index) {
    bool decl_res = false;

    Token_t token = ctx->tokens[ctx->i];
    Token_t next = (ctx->i + 1 < ctx->tokens_count) ? ctx->tokens[ctx->i + 1] : (Token_t){0};

    // add node to context
    size_t decl_index = add_node_to_context(ctx, NODE_DECLARATION, token.pos, token.line);
    Node_t* decl_node = &ctx->nodes[decl_index];
    Node_t* parent = &ctx->nodes[parent_index];

    decl_node->parent = parent_index;
    decl_node->scope_depth = parent->scope_depth + 1;
    decl_node->root_distance = parent->root_distance + 1;
    decl_node->scope_owner = scope_owner_index;

    // increment cursor
    ctx->i++;
    token = ctx->i < ctx->tokens_count ? ctx->tokens[ctx->i] : (Token_t){0};

    // check if the declaration is a variable or a constant
    decl_node->Declaration_data.is_var = token.kind == TOKEN_VAR ? true : false;

    if (token.kind == TOKEN_VAR) {
        decl_node->Declaration_data.is_var = true;
    } else if (token.kind == TOKEN_CONST) {
        decl_node->Declaration_data.is_var = false;
    } 

    // Expect an identifier
    ctx->i++;
    token = ctx->i < ctx->tokens_count ? ctx->tokens[ctx->i] : (Token_t){0};
    if (token.kind != TOKEN_IDENTIFIER) {
        size_t err_msg_len = strlen(en_us[RES_MISSING_STATEMENT_MSG]) + token.len + 1;
        char* err_msg = malloc(err_msg_len);
        if (err_msg == NULL) memory_allocation_failure(token.pos, token.line, ctx->filepath, __FILE__, __LINE__);

        snprintf(err_msg, err_msg_len, en_us[RES_MISSING_STATEMENT_MSG], get_substring(ctx->src, token.pos, token.len));
        add_error_to_context(ctx, en_us[RES_SYNTAX_ERROR_CAT], err_msg, token.pos, token.line, __FILE__, __LINE__);

        free(err_msg);
        return false;
    }

    // Store the identifier index
    decl_node->Declaration_data.identifier_index = add_node_to_context(ctx, NODE_IDENTIFIER, token.pos, token.line);

    // Expect an assignment operator
    ctx->i++;
    token = ctx->i < ctx->tokens_count ? ctx->tokens[ctx->i] : (Token_t){0};
    if (token.kind != TOKEN_ASSIGN) {
        size_t err_msg_len = strlen(en_us[RES_MISSING_STATEMENT_MSG]) + token.len + 1;
        char* err_msg = malloc(err_msg_len);
        if (err_msg == NULL) memory_allocation_failure(token.pos, token.line, ctx->filepath, __FILE__, __LINE__);

        snprintf(err_msg, err_msg_len, en_us[RES_MISSING_STATEMENT_MSG], get_substring(ctx->src, token.pos, token.len));
        add_error_to_context(ctx, en_us[RES_SYNTAX_ERROR_CAT], err_msg, token.pos, token.line, __FILE__, __LINE__);

        free(err_msg);
        return false;
    }

    // Parse the expression
    ctx->i++;
    decl_node->Declaration_data.expression_index = add_node_to_context(ctx, NODE_INTEGER, token.pos, token.line);
    decl_res = parse_expression(ctx, decl_node->Declaration_data.expression_index, scope_owner_index);

    return decl_res;
}

bool parse_expression(Transpiler_context_t* ctx, size_t parent_index, size_t scope_owner_index) {
    Token_t token = ctx->tokens[ctx->i];
    if (token.kind == TOKEN_INTEGER) {
        // Handle integer literal
        Node_t* expr_node = &ctx->nodes[parent_index];
        expr_node->kind = NODE_INTEGER;
        expr_node->Integer_data.len = token.len;
        ctx->i++;
        return true;
    }
    // Add more expression parsing logic here if needed
    return false;
}

void recover(Transpiler_context_t* ctx) {
    while (ctx->i < ctx->tokens_count && ctx->tokens[ctx->i].kind != TOKEN_EOF) {
        ctx->i++;
    }
}

bool parse_block(Transpiler_context_t* ctx, size_t parent_index, size_t scope_owner_index) {
    bool block_res = false;

    // the statements list in parent should be already allocated
    Node_t* parent = &ctx->nodes[parent_index];

    while (ctx->i < ctx->tokens_count) {
        Token_t token = ctx->tokens[ctx->i];
        Token_t next = (ctx->i + 1 < ctx->tokens_count) ? ctx->tokens[ctx->i + 1] : (Token_t){0};

        if (token.kind == TOKEN_LET) {
            printf("Parsing Declaration\n");
            bool decl_res = parse_declaration(ctx, parent_index, scope_owner_index);
            if (decl_res) {
                block_res = true;
            } else {
                recover(ctx);
            }
        } else {
            size_t err_msg_len = strlen(en_us[RES_MISSING_STATEMENT_MSG]) + token.len + 1;
            char* err_msg = malloc(err_msg_len);
            if (err_msg == NULL) memory_allocation_failure(token.pos, token.line, ctx->filepath, __FILE__, __LINE__);

            snprintf(err_msg, err_msg_len, en_us[RES_MISSING_STATEMENT_MSG], get_substring(ctx->src, token.pos, token.len));
            add_error_to_context(ctx, en_us[RES_SYNTAX_ERROR_CAT], err_msg, token.pos, token.line, __FILE__, __LINE__);

            free(err_msg);
            ctx->i++;
        }
    }

    return block_res;
}

void parse_file(Transpiler_context_t* ctx) {
    // set cursor
    ctx->i = 0;

    size_t index = add_node_to_context(ctx, NODE_PROGRAM, 0, 0);

    // create root node. As memory is already allocated, we can directly access the root node
    Node_t* root = &ctx->nodes[index];

    root->kind = NODE_PROGRAM;

    root->scope_depth = 0;
    root->root_distance = 0;

    root->parent = 0;
    root->scope_owner = 0;

    // Initialize the program node
    root->Program_data.statements_count = 0;
    root->Program_data.statements_capacity = 8;
    root->Program_data.statements = calloc(root->Program_data.statements_capacity, sizeof(size_t));
    if (root->Program_data.statements == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);

    bool res = parse_block(ctx, index, index);
}