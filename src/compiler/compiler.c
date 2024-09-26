#include <stdbool.h>
#include <time.h>

#include "pixel.h"
#include "compiler.h"

void leaf_print(Leaf* leaf, size_t indent) {
    for (size_t i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("{ kind: %s, value: %s, pos: %zu, line: %zu }\n", leaf->kind, leaf->value, leaf->pos, leaf->line);
    if (leaf->args) {
        for (size_t i = 0; i < leaf->args->length; i++) {
            Leaf* arg = (Leaf*)leaf->args->items[i];
            leaf_print(arg, indent + 1);
        }
    }
    if (leaf->statements) {
        for (size_t i = 0; i < leaf->statements->length; i++) {
            Leaf* statement = (Leaf*)leaf->statements->items[i];
            leaf_print(statement, indent + 1);
        }
    }
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
        printf("{ kind: %s, value: %s, pos: %zu, line: %zu }\n", token->kind, token->value, token->pos, token->line);
    }

    Leaf* program = (Leaf*)malloc(sizeof(Leaf));
    if (!program) {
        perror("Memory allocation failed for program");
        exit(EXIT_FAILURE);
    }
    program->kind = "PROGRAM";
    program->value = NULL;
    program->pos = 0;
    program->line = 0;
    program->args = NULL;
    program->statements = list_init("List<Leaf>");

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
    leaf_print(program, 0);
    

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("\n---------------------------------\n");
    printf("Compilation took %f ms to execute \n", cpu_time_used);        
}

// create an type a;ias for int
// typedef int myint;