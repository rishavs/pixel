#include "transpiler.h"

void transpile_file(TRANSPILER_CONTEXT* ctx) {

    // add dummy tokens & errors
    TOKEN t = {TOKEN_IDENTIFIER, "dummy", 0, 0};
    add_token_to_context(ctx, TOKEN_IDENTIFIER, "dummy", 0, 0);
    add_error_to_context(ctx, "SyntaxError", "Unexpected Token", "Unexpected Token", "test.c", 0, 0, __FILE__, __LINE__);

    // add dummy return value
    ctx->cFileCode = "\
#include <stdio.h>\
int main() {\
    return 0;\
}\
";
    
}