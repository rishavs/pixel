#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"
#include "transpiler.h"

// void recover(Transpiler_context_t* ctx) {
//     while (ctx->i < ctx->tokens_count && ctx->tokens[ctx->i].kind != TOKEN_EOF) {
//         ctx->i++;
//     }
// }

// Nodes_List parse_block(Transpiler_context_t* ctx) {

//     Nodes_List block = {
//         .nodes = calloc(block.capacity, sizeof(Node)),
//         .count = 0,
//         .capacity = 8
//     };
//     if (block.nodes == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

//     while (ctx->i < ctx->tokens_count) {
//         // in_recovery_loop = false;

//         TOKEN token = ctx->tokens[ctx->i];
//         TOKEN next = ctx->i + 1 < ctx->tokens_count ? ctx->tokens[ctx->i + 1] : NULL;

//         if (token.kind == TOKEN_CONST || token.kind == TOKEN_VAR) {
//             printf("Parsing Declaration\n");
//             // Node* decl_node = parse_declaration(ctx);
//             // if (!decl_node || decl_node->kind == NODE_ILLEGAL) recover(ctx);
//             // add_to_nodes_list(&block, decl_node);

//         // } else if (token->kind == TOKEN_IDENTIFIER && next_token && next_token->kind == TOKEN_ASSIGN) {
//         //     Node* decl_node = parse_declaration(ctx);
//         //     if (!decl_node) {
//         //         in_recovery_loop = true;
//         //         return block;
//         //     }
//         //     list_push(block, decl_node);

//         // } else if (token->kind == TOKEN_RETURN) {
//         //     Node* ret_node = parse_return(ctx);
//         //     if (!ret_node) {
//         //         in_recovery_loop = true;
//         //         return block;
//         //     }
//         //     list_push(block, ret_node);

//         } else {
//             add_error_to_context(ctx, "SyntaxError", "Expected a statement, but instead found: ", token.value, ctx->filepath, token.line, token.pos, __FILE__, __LINE__);
//             ctx->i++;
//         }
//     }

//     return block;
// }


void parse_file(Transpiler_context_t* ctx) {
    // set cursor
    ctx->i = 0;

    size_t index = add_node_to_context(ctx, NODE_PROGRAM, 0, 0);

    // create root node. As memory is already allocated, we can directly access the root node
    Node_t* root = &ctx->nodes[0];

    root->kind = NODE_PROGRAM;
    root->len = 0;

    root->scope_depth = 0;
    root->root_distance = 0;

    root->parent = 0;
    root->scope_owner = 0;

    root->args = NULL;
    root->args_count = 0;
    root->args_capacity = 0;

    root->children_count = 0;
    root->children_capacity = 8;
    root->children = calloc(root->children_capacity, sizeof(Node_t));
    if (root->children == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    
    // List* statements = parse_block(ctx);
    // for (size_t j = 0; j < statements->length; j++) {
    //     Node* stmt = statements->items[j];
    //     stmt->parent = program;
    //     stmt->depth = 1;
    // }

    // printf("\nNumber of Statements: %zu\n", statements->length);
    // program->Node_Program.block = statements;


}