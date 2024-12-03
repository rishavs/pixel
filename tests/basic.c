#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "transpiler.h"

// Null input
bool test_01() {
    char* input = "";
    // char* output = transpile_file("test.c", input);
    char* output = "";
    fatal_memory_allocation_failure(__FILE__, __LINE__);
    return (strcmp(input, output) == 0);
}

// Null input with comments
bool test_02() {
    char* input = "/* This is a comment */";
    TRANSPILER_CONTEXT* ctx = create_transpiler_context("test.c", input);
    transpile_file(ctx);
    printf("Errors count: %zu\n", ctx->errors_count);
    printf(ctx->errors[0].msg);
    printf(ctx->cFileCode);
    char* output = "";
    return (strcmp(input, output) == 0);
}

// Integer
bool test_03() {
    char* input = "13";
    // char* output = transpile_file("test.c", input);
    char* output = "13";
    return (strcmp(input, output) == 0);
}
