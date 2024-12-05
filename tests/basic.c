#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "tests.h"
#include "transpiler.h"

// Null input
Test_Result test_01() {
    Test_Result res = { 
        .desc = "Empty source code",
        .passed = false 
    };
    char* src = "";
    char* output = DEFAULT_CFILE_CODE;

    TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
    ctx->src = src;
    transpile_file(ctx);
    res.passed = strcmp(ctx->cFileCode, output) == 0;

    return res;
}

// Null input with comments
Test_Result test_02() {
    Test_Result res = { 
        .desc = "Empty source code with comments",
        .passed = false 
    };

    char* src = " \
    -- This is a single line comment \
    -[ This is a multi \
    \
    line comment \
]- ";
    char* output = DEFAULT_CFILE_CODE;

    TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
    ctx->src = src;
    transpile_file(ctx);

    res.passed = strcmp(ctx->cFileCode, output) == 0;

    return res;
}

// variable declaration for Integer
Test_Result test_03() {
    Test_Result res = { 
        .desc = "TODO: Basic variable declaration for Integer",
        .passed = false 
    };

    char* src = "var x = 10";
    char* output = "#include <stdint.h>\
\
int main() {\
    int64_t x = 10;\
    return 0;\
}";

    TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
    ctx->src = src;
    transpile_file(ctx);
    
    res.passed = strcmp(ctx->cFileCode, output) == 0;

    return res;
}
