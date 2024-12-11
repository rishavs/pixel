#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "helpers.h"
#include "resources.h"
#include "errors.h"
#include "transpiler.h"

size_t parse_integer(Transpiler_context_t* ctx) {
    Token_t token = ctx->tokens[ctx->i];
    size_t index = add_node_to_context(ctx, NODE_INTEGER, token.pos, token.line);

    return index;
}

size_t parse_identifier(Transpiler_context_t* ctx) {
    Token_t token = ctx->tokens[ctx->i];
    size_t index = add_node_to_context(ctx, NODE_IDENTIFIER, token.pos, token.line);

    return index;
}

size_t parse_expression(Transpiler_context_t* ctx) {
    Token_t token = ctx->tokens[ctx->i];
    if (token.kind == TOKEN_INTEGER) {
        return parse_integer(ctx);
    }
    // Add more expression parsing logic here if needed

    parser_expected_syntax_error(ctx, en_us[RES_EXPRESSION], __FILE__, __LINE__);
    return 0;
}

size_t parse_declaration(Transpiler_context_t* ctx) {
    Token_t token = ctx->tokens[ctx->i];
    size_t index = add_node_to_context(ctx, NODE_DECLARATION, token.pos, token.line);
    Node_t* node = &ctx->nodes[index];

    
    // increment cursor. consume 'let' token
    ctx->i++;
    token = ctx->tokens[ctx->i];
    // next token should be identifier
    if (token.kind != TOKEN_IDENTIFIER) {
        printf("Expected identifier, but found %s\n", list_of_token_kinds[token.kind]);
        parser_expected_syntax_error(ctx, en_us[RES_IDENTIFIER], __FILE__, __LINE__);
        
        return 0;
    }

    size_t identifier_index = parse_identifier(ctx);
    if (identifier_index == 0) {
        return 0;
    }
    node->Declaration_data.identifier_index = identifier_index;

    // increment cursor. consume identifier token
    ctx->i++;
    token = ctx->tokens[ctx->i];

    // next token should be assignment operator
    if (token.kind != TOKEN_ASSIGN) {
        parser_expected_syntax_error(ctx, "=", __FILE__, __LINE__);
        
        return 0;
    }

    // increment cursor. consume '=' token
    ctx->i++;
    token = ctx->tokens[ctx->i];

    // parse expression
    size_t expression_index = parse_expression(ctx);
    if (expression_index == 0) {
        return 0;
    }

    node->Declaration_data.expression_index = expression_index;

    return index;       
}

size_t parse_statement(Transpiler_context_t* ctx) {
    Token_t token = ctx->tokens[ctx->i];
    if (token.kind == TOKEN_LET) {
        return parse_declaration(ctx);
    } 

    // if no other statement is found, return 0
    parser_expected_syntax_error(ctx, en_us[RES_STATEMENT], __FILE__, __LINE__);
    return 0;
}

void recover_from_error(Transpiler_context_t* ctx) {
    // skip to the next "End" token
    while (ctx->i < ctx->tokens_count) {
        Token_t token = ctx->tokens[ctx->i];
        if (token.kind == TOKEN_END) {
            break;
        }
        ctx->i++;
    }
}

size_t* parse_block(Transpiler_context_t* ctx) {
    Token_t token = ctx->tokens[ctx->i];
    Token_t next = (ctx->i + 1 < ctx->tokens_count) ? ctx->tokens[ctx->i + 1] : (Token_t){0};

    size_t block_count = 0;
    size_t block_capacity = 8;
    size_t* block_list = calloc(block_capacity, sizeof(size_t));
    if (block_list == NULL) memory_allocation_failure(token.pos, token.line, ctx->filepath, __FILE__, __LINE__);

    while (ctx->i < ctx->tokens_count) {
        token = ctx->tokens[ctx->i];
        next = (ctx->i + 1 < ctx->tokens_count) ? ctx->tokens[ctx->i + 1] : (Token_t){0};

        if (token.kind == TOKEN_LET) {
            size_t stmt_index = parse_statement(ctx);
            add_to_indices(block_list, stmt_index, block_count, block_capacity);

            if (stmt_index == 0) {
                recover_from_error(ctx);
            }
        } else {
            parser_expected_syntax_error(ctx, en_us[RES_STATEMENT], __FILE__, __LINE__);
            recover_from_error(ctx);

        }
        ctx->i++;
    }

    return block_list;
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
    root->Program_data.statements = parse_block(ctx);

}