#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ast.h"
#include "compiler.h"
#include "helpers.h"


typedef struct {
    int indent_level;

    // Std C Headers
    bool c_stdlib;
    bool c_stdio;
    bool c_string;
    bool c_stdint;

    // vendor headers
    bool gc;

    // pixel headers
    bool pixel_core;
    bool pixel_dict;
    bool pixel_list;
    bool pixel_string;
    bool pixel_utils;

} Codegen_context;

char* generate_indent(Codegen_context* ctx) {
    char* indent = (char*)malloc(ctx->indent_level * 4 + 1);
    if (!indent) {
        perror("Memory allocation failed for indentation");
        exit(EXIT_FAILURE);
    }
    indent[0] = '\0'; // Initialize the string
    for (int i = 0; i < ctx->indent_level; i++) {
        strcat(indent, "    ");  
    }
    return indent;
}

char* generate_filename (char* filepath, bool is_module) {
    if (is_module) {
        // Convert the full filepath to a URI-safe string
        char* uri_safe_filepath = to_uri_safe_string(filepath);
        if (!uri_safe_filepath) {
            return NULL;
        }
        return uri_safe_filepath;
    } else {
        // Extract the final filename and change the extension to .c
        const char* filename_start = strrchr(filepath, '/');
        if (filename_start == NULL) {
            filename_start = filepath; // If no '/' found, the filename starts at the beginning
        } else {
            filename_start++; // Skip the '/'
        }

        size_t filename_length = strlen(filename_start);
        char* filename = (char*)malloc(filename_length + 3 + 1); // Allocate memory for the filename with .c extension
        if (filename == NULL) {
            perror("Failed to allocate memory for filename");
            return NULL;
        }
        strcpy(filename, filename_start);
        return filename;
    }
}

// ------------------------------------------------
// Codegen functions for AST nodes
// ------------------------------------------------

char* generate_expression(Codegen_context* ctx, Node* expr);

char* generate_integer (Codegen_context* ctx, Node* int_node) {
    return int_node->Node_Integer.value;
}
char* generate_decimal (Codegen_context* ctx, Node* dec_node) {
    return dec_node->Node_Decimal.value;
}

char* generate_identifier(Codegen_context* ctx, Node* ident_node) {
    return ident_node->Node_Identifier.value;
}

char* generate_unary_expression(Codegen_context* ctx, Node* unary_expr) {
    char* operator = unary_expr->Node_Unary.operator;
    char* right = generate_expression(ctx, unary_expr->Node_Unary.right);
    return join_strings(4, "(", operator, right, ")");
}
char* generate_binary_expression(Codegen_context* ctx, Node* binary_expr) {
    char* operator = binary_expr->Node_Binary.operator;
    char* result = generate_expression(ctx, binary_expr->Node_Binary.expressions->items[0]);
    for (size_t i = 1; i < binary_expr->Node_Binary.expressions->length; i++) {
        char* expr = generate_expression(ctx, binary_expr->Node_Binary.expressions->items[i]);
        result = join_strings(5, result, " ", operator, " ", expr);
    }
    result = join_strings(3, "(", result, ")");
    return result;
}

char* generate_expression(Codegen_context* ctx, Node* expr) {
    switch (expr->kind) {
        case NODE_IDENTIFIER:
            return generate_identifier(ctx, expr);
        case NODE_INTEGER:
            return generate_integer(ctx, expr);
        case NODE_DECIMAL:
            return generate_decimal(ctx, expr);
        case NODE_UNARY:
            return generate_unary_expression(ctx, expr);
        case NODE_BINARY:
            return generate_binary_expression(ctx, expr);
        default:
            perror("Codegen failed. Unknown expression type");
            return NULL;
    }
}

// TODO - types!!!
char* generate_declaration(Codegen_context* ctx, Node* decl_stmt) {
    char* is_var = decl_stmt->Node_Declaration.is_var ? "" : "const ";
    char* declared_type = decl_stmt->Node_Declaration.is_new ? "int " : "";
    char* identifier = generate_identifier(ctx, decl_stmt->Node_Declaration.identifier);
    char* expr = decl_stmt->Node_Declaration.is_assignment ? generate_expression(ctx, decl_stmt->Node_Declaration.expr) : "0";
    return join_strings(7, generate_indent(ctx), declared_type, is_var, identifier, " = ", expr, ";");
}

char* generate_return_statement(Codegen_context* ctx, Node* ret_stmt) {
    char* expr = generate_expression(ctx, ret_stmt->Node_Return.expr);
    return join_strings(4, generate_indent(ctx), "return ", expr, ";");
}

char* generate_statement(Codegen_context* ctx, Node* stmt) {
    switch (stmt->kind) {
        case NODE_DECLARATION:
            printf("Generating declaration, %s\n", stmt->Node_Declaration.identifier->Node_Identifier.value);
            return generate_declaration(ctx, stmt);
        case NODE_RETURN:
            return generate_return_statement(ctx, stmt);
        default:
            perror("Codegen failed. Unknown statement type");
            exit(EXIT_FAILURE);
    }
}

// scope boundary
char* generate_block(Codegen_context* ctx, List* stmts) {
    char* block = "";

    // Increase the indentation level on block start
    ctx->indent_level++;

    for (size_t i = 0; i < stmts->length; i++) {
        char* stmt = generate_statement(ctx, stmts->items[i]);
        block = join_strings(3, block, stmt, "\n");
    }

    // Decrease the indentation level on block end
    ctx->indent_level--;

    return block;
}

char* generate_program(Codegen_context* ctx, Node* program) {
    char* block = generate_block(ctx, program->Node_Program.block);
    return block;
}

// ------------------------------------------------
// Codegen functions for AST nodes
// ------------------------------------------------

char* generate_headers (Codegen_context* ctx) {
    printf("Generating headers\n");
    char* headers = "// headers\n";
    if (ctx->c_stdlib)  headers = join_strings(2, headers, "#include <stdlib.h>\n");
    if (ctx->c_stdio)   headers = join_strings(2, headers, "#include <stdio.h>\n");
    if (ctx->c_string)  headers = join_strings(2, headers, "#include <string.h>\n");
    if (ctx->c_stdint)  headers = join_strings(2, headers, "#include <stdint.h>\n");

    headers = join_strings(2, headers, "\n");
    printf("Headers have been generated\n");

    return headers;
}

// TODO - Generate core builtins, like type defs. copy to the output folder for including
void generate_includes() {}

void generate_code (Node* program, char* filepath) {
    Codegen_context* ctx = (Codegen_context*)malloc(sizeof(Codegen_context));
    if (!ctx) {
        perror("Memory allocation failed for codegen context");
        exit(EXIT_FAILURE);
    }
    ctx->indent_level = 0;

    ctx->c_stdlib = true;
    ctx->c_stdio = false;
    ctx->c_string = false;
    ctx->c_stdint = false;

    ctx->gc = false;
    
    ctx->pixel_core = false;
    ctx->pixel_dict = false;
    ctx->pixel_list = false;
    ctx->pixel_string = false;
    ctx->pixel_utils = false;

    char* input_filename = generate_filename(filepath, false);
    printf("Generating code for %s\n", input_filename);

    char* dotc_filename = join_strings(3, "./oven/", input_filename, ".c");
    printf("Output file: %s\n", dotc_filename);

    FILE* dotc_file_handler = fopen(dotc_filename, "w");
    if (!dotc_file_handler) {
        perror("Failed to open output file");
        return;
    }

    char* headers = generate_headers(ctx);
    if (!headers) {
        perror("Failed to generate headers");
        fclose(dotc_file_handler);
        return;
    }
    printf("Headers:\n%s\n", headers);

    char* code = generate_program(ctx, program);
    if (!code) {
        perror("Failed to generate code");
        free(headers);
        fclose(dotc_file_handler);
        return;
    }
    printf("Code:\n%s\n", code);

    printf("Writing to file\n");

    if (fprintf(dotc_file_handler, "%s", headers) < 0) {
        perror("Failed to write headers to file");
    }
    if (fprintf(dotc_file_handler, "void main() {\n") < 0) {
        perror("Failed to write main function start to file");
    }
    if (fprintf(dotc_file_handler, "%s", code) < 0) {
        perror("Failed to write code to file");
    }
    if (fprintf(dotc_file_handler, "}\n") < 0) {
        perror("Failed to write main function end to file");
    }

    fclose(dotc_file_handler);
    free(headers);
    free(code);
}