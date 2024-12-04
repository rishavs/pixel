#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "transpiler.h"

// Null input
bool test_01() {
    char* src = "";
    TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
    ctx->src = src;
    transpile_file(ctx);
    char* output = DEFAULT_CFILE_CODE;
    return (strcmp(ctx->cFileCode, output) == 0);
}

// Null input with comments
bool test_02() {
    char* input = " \
    -- This is a single line comment \
    -[ This is a multi \
    \
    line comment \
]- ";
    TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
    ctx->src = input;
    transpile_file(ctx);
    char* output = DEFAULT_CFILE_CODE;
    return (strcmp(ctx->cFileCode, output) == 0);
}

// variable declaration for Integer
bool test_03() {
    char* input = "var x = 10";
    TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
    ctx->src = input;
    transpile_file(ctx);
    char* output = "#include <stdint.h>\
\
int main() {\
    int64_t x = 10;\
    return 0;\
}";
    return (strcmp(ctx->cFileCode, output) == 0);
}
