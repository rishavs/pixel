#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"
#include "transpiler.h"


// bool parse_declaration(Transpiler_context_t* ctx, size_t parent_index) {
//     bool decl_res = false;

//     Token_t token = ctx->tokens[ctx->i];

//     // add node to context
//     size_t decl_index = add_node_to_context(ctx, NODE_DECLARATION, token.pos, token.line);
//     Node_t* decl_node = &ctx->nodes[decl_index];

//     // increment cursor
//     ctx->i++;
//     if 
//     token = ctx->i < ctx->tokens_count ? ctx->tokens[ctx->i] : NULL;

//     // check if the declaration is a variable or a constant
//     decl_node->Declaration_data.is_var = token.kind == TOKEN_VAR ? true : false;

//     if (token.kind == TOKEN_VAR) {
//         decl_node->Declaration_data.is_var = true;
//     } else if (token.kind == TOKEN_CONST) {
//         decl_node->Declaration_data.is_var = false;
//     } 

//     if (token.kind == TOKEN_LET) {
//         size_t index = add_node_to_context(ctx, NODE_DECLARATION, token.pos, token.line);
//         Node_t* decl_node = &ctx->nodes[index];

//         decl_node->Declaration_data = (Declaration_data_t) {
//             .is_var = token.kind == TOKEN_VAR,
//             .is_new = false,
//             .is_assignment = false,
//             .identifier_index = 0,
//             .expression_index = 0
//         };

//         decl_node->parent = parent_index;
//         decl_node->scope_depth = 0;
//         decl_node->root_distance = 0;
//         decl_node->scope_owner = 0;

//         // add_to_indices(parent->Program_data.statements, index, parent->Program_data.statements_count, parent->Program_data.statements_capacity);
//         decl_res = true;
//     }

//     return decl_res;
// }


// void recover(Transpiler_context_t* ctx) {
//     while (ctx->i < ctx->tokens_count && ctx->tokens[ctx->i].kind != TOKEN_EOF) {
//         ctx->i++;
//     }
// }

// bool parse_block(Transpiler_context_t* ctx, size_t parent_index) {
//     bool block_res = false;

//     // the statements list in parent should be already allocated
//     Node_t* parent = &ctx->nodes[parent_index];

//     while (ctx->i < ctx->tokens_count) {
//         // in_recovery_loop = false;

//         Token_t token = ctx->tokens[ctx->i];
//         Token_t next = ctx->i + 1 < ctx->tokens_count ? ctx->tokens[ctx->i + 1] : NULL;

//         if (token.kind == TOKEN_LET) {
//             printf("Parsing Declaration\n");
//             bool decl_res = parse_declaration(ctx, parent_index);
//             if (!decl_res) {
//                 recover(ctx);
//             }
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
//             char* buffer = calloc(token.len + 1, sizeof(char));
//             if (buffer == NULL) memory_allocation_failure(__FILE__, __LINE__);
//             strncpy(buffer, ctx->src + token.pos, token.len);
//             buffer[token.len] = '\0';
//             add_error_to_context(ctx, "SyntaxError", "Expected a statement, but instead found: ", buffer, ctx->filepath, token.line, token.pos, __FILE__, __LINE__);
//             ctx->i++;
//         }
//     }

//     return block_res;
// }


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
    // root->Program_data = (Program_data_t) {
    //     .statements = NULL,
    //     .statements_count = 0,
    //     .statements_capacity = 0
    // };
    // root->Program_data.statements = calloc(root->Program_data.statements_capacity, sizeof(size_t));
    // if (root->Program_data.statements == NULL) memory_allocation_failure(__FILE__, __LINE__);

    // List* statements = parse_block(ctx);
    // for (size_t j = 0; j < statements->length; j++) {
    //     Node* stmt = statements->items[j];
    //     stmt->parent = program;
    //     stmt->depth = 1;
    // }

    // printf("\nNumber of Statements: %zu\n", statements->length);
    // program->Node_Program.block = statements;


}