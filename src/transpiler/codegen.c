#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "resources.h"
#include "errors.h"
#include "helpers.h"
#include "transpiler.h"

char* indent(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];

    size_t indent_size = node->scope_depth * 4 + 1; // 4 spaces per scope depth + null terminator

    char* indent = calloc(indent_size, sizeof(char));
    if (indent == NULL) memory_allocation_failure(node->pos, node->line, ctx->filepath, __FILE__, __LINE__);

    for (size_t i = 0; i < node->scope_depth; i++) {
        strcat(indent, "    ");
    }    

    return indent;
}

char* gen_integer(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];

    ctx->has_integer = true;
    return get_substring(ctx->src, node->pos, node->Integer_data.len);
}

char* gen_identifier(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];

    return get_substring(ctx->src, node->pos, node->Identifier_data.len);
}

char* gen_expression(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];

    if (ctx->nodes[index].kind == NODE_INTEGER) {
        return gen_integer(ctx, index);
    }
    // Add more expression parsing logic here if needed

    unhandled_error(ctx->nodes[index].pos, ctx->nodes[index].line, ctx->filepath, __FILE__, __LINE__);
    return "";
}

// TODO - handle types!
char* gen_declaration(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];
    size_t identifier_index = node->Declaration_data.identifier_index;
    size_t expression_index = node->Declaration_data.expression_index;
    size_t scope_depth = node->scope_depth;

    char* code = join_strings(6,
        indent(ctx, index),
        ctx->tokens[ctx->i].kind == TOKEN_LET ? "int " : "",
        gen_identifier(ctx, identifier_index),
        ctx->tokens[ctx->i].kind == TOKEN_ASSIGN ? " = " : "",
        gen_expression(ctx, expression_index),
        ";\n"
    );

    return code;
}

// add trailing semicolon to all statements
char* gen_statement(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];

    if (ctx->tokens[ctx->i].kind == TOKEN_LET) {
        return gen_declaration(ctx, index);
    } 

    // if no other statement is found, return 0
    unhandled_error(ctx->nodes[index].pos, ctx->nodes[index].line, ctx->filepath, __FILE__, __LINE__);
    return "";
}

char* gen_block(Transpiler_context_t* ctx, size_t index) {
    Node_t* node = &ctx->nodes[index];
    size_t statements_count = node->Program_data.statements_count;

    char* temp_code = calloc(1, sizeof(char));
    if (temp_code == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);

    for (size_t i = 0; i < statements_count; i++) {
        char* statement_code = gen_statement(ctx, index);
        temp_code = realloc(temp_code, strlen(temp_code) + strlen(statement_code) + 2);
        strcat(temp_code, statement_code);
        strcat(temp_code, "\n");
        free(statement_code);
    }

    return temp_code;
}

char* gen_h_code(Transpiler_context_t* ctx) {
    return "";
}


char* gen_c_code(Transpiler_context_t* ctx) {
    // Node_t* program = &ctx->nodes[0];

    char* imports = "#include <stdlib.h>\n";
    if (ctx->has_integer) {
        imports = join_strings(2, imports, "#include <stdint.h>\n");
    }

    char* code = join_strings(6, 
        imports,
        "int main() {\n",
        gen_block(ctx, 0),
        "\n    return ",
        build_string("%d", ctx->exit_code),
        "\n}"
    );

    return code;
}


void gen_code(Transpiler_context_t* ctx) {
    // reset cursor
    ctx->i = 0;
    ctx->exit_code = 0;

    ctx->c_code = strdup(gen_c_code(ctx));
    ctx->h_code = strdup(gen_h_code(ctx));
}