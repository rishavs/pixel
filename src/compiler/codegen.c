#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "compiler.h"

// Function to generate C99 code from an AST node
void generate_code(Node* node, FILE* output) {
    if (!node) return;

    switch (node->kind) {
        case NODE_INTEGER:
            fprintf(output, "%s", node->Node_Integer.value);
            break;
        case NODE_DECIMAL:
            fprintf(output, "%s", node->Node_Decimal.value);
            break;
        case NODE_UNARY:
            fprintf(output, "%s", node->Node_Unary.operator);
            generate_code(node->Node_Unary.right, output);
            break;
        case NODE_BINARY:
            generate_code(node->Node_Binary.left, output);
            fprintf(output, " %s ", node->Node_Binary.operator);
            generate_code(node->Node_Binary.right, output);
            break;
        case NODE_RETURN:
            fprintf(output, "return ");
            generate_code(node->Node_Return.expr, output);
            fprintf(output, ";\n");
            break;
        case NODE_PROGRAM:
            for (size_t i = 0; i < node->Node_Program.block->length; i++) {
                generate_code(node->Node_Program.block->items[i], output);
            }
            break;
        default:
            fprintf(stderr, "Unknown node type: %d\n", node->kind);
            break;
    }
}

void generate_integer () {}
void generate_decimal () {}
void generate_grouping_expression() {}
void generate_unary_expression() {}
void generate_binary_expression() {}

void generate_expression() {}

void generate_return_statement() {}

void generate_statements() {}


// scope boundary
void generate_block() {}

// Function to generate the main function wrapper
void generate_main_function(Node* program, FILE* output) {
    fprintf(output, "#include <stdio.h>\n\n");
    fprintf(output, "int main() {\n");
    generate_code(program, output);
    fprintf(output, "    return 0;\n");
    fprintf(output, "}\n");
}

void generate_doth_file() {}

void generate_headers (Node* node, FILE* output) {
    fprintf(output, "#include <stdio.h>\n\n");
}

// Generate core builtins, like type defs. copy to the output folder for including
void generate_includes() {}

void generate_dotc_file (char* filepath) {
    FILE* output = fopen(filepath, "w");
    if (!output) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    fclose(output);
}

char* generate_filename (char* filepath) {
    const char* filename_start = strrchr(filepath, '/');
    if (filename_start == NULL) {
        filename_start = filepath; // If no '/' found, the filename starts at the beginning
    } else {
        filename_start++; // Skip the '/'
    }

    size_t filename_length = strlen(filename_start);
    char* filename = (char*)malloc(filename_length + 1); // Allocate memory for the filename
    if (filename == NULL) {
        perror("Failed to allocate memory for filename");
        return NULL;
    }
    strcpy(filename, filename_start);
    return filename;

}

void generate_file (Node* program, char* filepath) {
    char* filename = generate_filename(filepath);

}