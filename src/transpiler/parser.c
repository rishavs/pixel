#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"
#include "transpiler.h"

void recover(TRANSPILER_CONTEXT* ctx) {
    while (ctx->i < ctx->tokens_count && ctx->tokens[ctx->i].kind != TOKEN_EOF) {
        ctx->i++;
    }
}

Nodes_List parse_block(TRANSPILER_CONTEXT* ctx) {

    Nodes_List block = {
        .nodes = calloc(block.capacity, sizeof(Node)),
        .count = 0,
        .capacity = 8
    };
    if (block.nodes == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    while (ctx->i < ctx->tokens_count) {
        // in_recovery_loop = false;

        TOKEN token = ctx->tokens[ctx->i];
        TOKEN next = ctx->i + 1 < ctx->tokens_count ? ctx->tokens[ctx->i + 1] : NULL;

        if (token.kind == TOKEN_CONST || token.kind == TOKEN_VAR) {
            printf("Parsing Declaration\n");
            // Node* decl_node = parse_declaration(ctx);
            // if (!decl_node || decl_node->kind == NODE_ILLEGAL) recover(ctx);
            // add_to_nodes_list(&block, decl_node);

        // } else if (token->kind == TOKEN_IDENTIFIER && next_token && next_token->kind == TOKEN_ASSIGN) {
        //     Node* decl_node = parse_declaration(ctx);
        //     if (!decl_node) {
        //         in_recovery_loop = true;
        //         return block;
        //     }
        //     list_push(block, decl_node);

        // } else if (token->kind == TOKEN_RETURN) {
        //     Node* ret_node = parse_return(ctx);
        //     if (!ret_node) {
        //         in_recovery_loop = true;
        //         return block;
        //     }
        //     list_push(block, ret_node);

        } else {
            add_error_to_context(ctx, "SyntaxError", "Expected a statement, but instead found: ", token.value, ctx->filepath, token.line, token.pos, __FILE__, __LINE__);
            ctx->i++;
        }
    }

    return block;
}


void parse_file(TRANSPILER_CONTEXT* ctx) {

    // create root node
    Node* root = calloc(1, sizeof(Node));
    if (root == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    root->kind = NODE_PROGRAM;
    root->pos = 0;
    root->line = 0;
    root->scope_depth = 0;
    root->root_distance = 0;
    root->parent = NULL;
    root->scope_owner = NULL;
    root->Node_Program.statements = NULL;
    root->Node_Program.statements_count = 0;
    root->Node_Program.statements_capacity = 0;

    // List* statements = parse_block(ctx);
    // for (size_t j = 0; j < statements->length; j++) {
    //     Node* stmt = statements->items[j];
    //     stmt->parent = program;
    //     stmt->depth = 1;
    // }

    // printf("\nNumber of Statements: %zu\n", statements->length);
    // program->Node_Program.block = statements;

    // set cursor
    ctx->i = 0;

    ctx->root = root;
}