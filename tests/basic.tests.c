#include <stdbool.h>
#include <string.h>

#include "tests.h"
#include "transpiler.h"

// Null input
Test_Result test_01() {
    Test_Result res = { 
        .desc = "Empty source code",
        .passed = false 
    };
    char* src = "";
    char* c_out = DEFAULT_CFILE_CODE;
    char* h_out = "";
    
    Transpiler_context_t ctx;
    create_Transpiler_context_t(&ctx, "");
    ctx.src = strdup(src);
    transpile_file(&ctx);
    res.passed = strcmp(ctx.c_code, c_out) == 0 && strcmp(ctx.h_code, h_out) == 0;

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
    char* c_out = DEFAULT_CFILE_CODE;
    char* h_out = "";
    
    Transpiler_context_t ctx;
    create_Transpiler_context_t(&ctx, "");
    ctx.src = strdup(src);
    transpile_file(&ctx);
    res.passed = strcmp(ctx.c_code, c_out) == 0 && strcmp(ctx.h_code, h_out) == 0;
    
    return res;
}

// variable declaration for Integer
Test_Result test_03() {
    Test_Result res = { 
        .desc = "TODO: Basic variable declaration for Integer",
        .passed = false 
    };

    char* src = "var x = 10";
    char* c_out = "#include <stdint.h>\
\
int main() {\
    int64_t x = 10;\
    return 0;\
}";
    char* h_out = "";
    
    Transpiler_context_t ctx;
    create_Transpiler_context_t(&ctx, "");
    ctx.src = strdup(src);
    transpile_file(&ctx);
    res.passed = strcmp(ctx.c_code, c_out) == 0 && strcmp(ctx.h_code, h_out) == 0;
    
    return res;
}
