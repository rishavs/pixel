#include <stdbool.h>
#include <time.h>

#include "pixel.h"
#include "compiler.h"
#include "ast.h"

// Forward declaration
void print_ast_node(Node* node, int indent_level);

// Helper function to print indentation
void print_indent(int indent_level) {
    for (int i = 0; i < indent_level; i++) {
        printf("    ");  // Two spaces for each indent level
    }
}

// Function to print a list of nodes (for block statements)
void print_node_list(List* list, int indent_level) {
    if (list == NULL) {
        print_indent(indent_level);
        printf("NULL\n");
        return;
    }
    for (size_t i = 0; i < list->length; i++) {
        print_ast_node(list->items[i], indent_level);
    }
}

// Main function to print AST nodes
void print_ast_node(Node* node, int indent_level) {
    if (node == NULL) {
        print_indent(indent_level);
        printf("NULL\n");
        return;
    }

    print_indent(indent_level);
    printf("%s : line %zu, pos %zu, parent: %s, depth: %d, scope_owner: %s", NodeKindStrings[node->kind], node->line, node->pos, node->parent ? NodeKindStrings[node->parent->kind] : "" , node->depth, node->scope_owner ? NodeKindStrings[node->scope_owner->kind] : "");

    if (node->kind == NODE_INTEGER) {
        printf(", Value: %s\n", node->Node_Integer.value);
        // printf(")\n");
    }
    else if (node->kind == NODE_DECIMAL || node->kind == NODE_INTEGER ) {
        printf("Value: %s\n", node->Node_Decimal.value);
    }
    else if (node->kind == NODE_UNARY) {
        printf(", Operator '%s'\n", node->Node_Unary.operator);
        print_indent(indent_level + 1);
        printf("Right:\n");
        print_ast_node(node->Node_Unary.right, indent_level + 1);
    }
    else if (node->kind == NODE_BINARY) {
        printf(", Operator: '%s'\n", node->Node_Binary.operator);
        print_indent(indent_level + 1);
        printf("Expressions:\n");
        print_node_list(node->Node_Binary.expressions, indent_level + 1);
    }
    else if (node->kind == NODE_DECLARATION) {
        printf("\n");
        print_indent(indent_level + 1);
        printf("Identifier: %s\n", node->Node_Declaration.identifier->Node_Identifier.value);
        if (node->Node_Declaration.is_new) {
            print_indent(indent_level + 1);
            printf("IS_NEW\n");
        }
        if (node->Node_Declaration.is_assignment) {
            print_indent(indent_level + 1);
            printf("Expression:\n");
            print_ast_node(node->Node_Declaration.expr, indent_level + 1);
        }
    }
    else if (node->kind == NODE_RETURN) {
        printf("\n");
        print_indent(indent_level + 1);
        printf("Expression:\n");
        print_ast_node(node->Node_Return.expr, indent_level + 1);
    }
    else if (node->kind == NODE_PROGRAM) {
        printf(", Filepath: %s\n", node->Node_Program.filepath);
        print_indent(indent_level + 1);
        printf("Block:\n");
        print_node_list(node->Node_Program.block, indent_level + 1);
    }
    else {
        print_indent(indent_level);
        printf("Unknown node type\n");
    }

}

// Usage example
void print_ast(Node* root) {
    print_ast_node(root, 0);
}

void compile_file (char* filepath) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    bool ok = false;
    List* errors = list_init("List<Error>");

    char* src;
    ok = read_file (errors, &src, filepath);
    if (!ok) {
        printf("Errors:\n");
        for (size_t i = 0; i < errors->length; i++) {
            CompilerError* error = (CompilerError*)errors->items[i];
            compiler_error_print(error);
        }
        return;
    }
    printf("Source code:\n---------------------------------\n");
    printf("%s", src);

    List* tokens = list_init("List<Token>");
    if (!tokens) {
        perror("Memory allocation failed on list_init");
        exit(EXIT_FAILURE);
    }
    ok = lex_file (errors, tokens, src, filepath);
    if (!ok) {
        printf("\nErrors:\n");
        for (size_t i = 0; i < errors->length; i++) {
            CompilerError* error = (CompilerError*)errors->items[i];
            compiler_error_print(error);
        }
        return;
    }
    printf("\n---------------------------------\n");
    printf("Tokens:\n---------------------------------\n");
        for (size_t i = 0; i < tokens->length; i++) {
        Token* token = (Token*)tokens->items[i];
        printf("{ kind: %s, value: %s, pos: %zu, line: %zu }\n", TokenKindStrings[token->kind], token->value, token->pos, token->line);
    }

    // Leaf* program = (Leaf*)malloc(sizeof(Leaf));
    // if (!program) {
    //     perror("Memory allocation failed for program");
    //     exit(EXIT_FAILURE);
    // }
    // program->kind = "PROGRAM";
    // program->value = NULL;
    // program->pos = 0;
    // program->line = 0;
    // program->args = NULL;
    // program->statements = list_init("List<Leaf>");

    
    Node* program = (Node*)malloc(sizeof(Node));
    if (!program) {
        perror("Failed to allocate memory for file node");
        exit(EXIT_FAILURE);
    }
    program->kind = NODE_PROGRAM;
    program->line = 0;
    program->pos = 0;
    program->parent = NULL;
    program->depth = 0;
    program->Node_Program.filepath = filepath;
    program->Node_Program.block = NULL;

    ok = parse_file (errors, program, tokens, filepath);
    
    if (!ok) {
        printf("\nErrors:\n");
        for (size_t i = 0; i < errors->length; i++) {
            CompilerError* error = (CompilerError*)errors->items[i];
            compiler_error_print(error);
        }
        return;
    }
    printf("\n---------------------------------\n");
    printf("Program:\n---------------------------------\n");
    print_ast(program);

    generate_code(program, filepath);    

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("\n---------------------------------\n");
    printf("Compilation took %f ms to execute \n", cpu_time_used);        
}

// create an type a;ias for int
// typedef int myint;