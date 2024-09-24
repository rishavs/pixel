#include <stdbool.h>
#include <time.h>

#include "pixel.h"
#include "compiler.h"

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
    if (!tokens) perror("Memory allocation failed on list_init");
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
        return;
    }
    program->kind = "PROGRAM";
    program->value = NULL;
    program->pos = 0;
    program->line = 0;
    program->args = NULL;
    program->statements = list_init("List<Leaf>");
    

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("\n---------------------------------\n");
    printf("Compilation took %f ms to execute \n", cpu_time_used);        
}

// create an type a;ias for int
// typedef int myint;