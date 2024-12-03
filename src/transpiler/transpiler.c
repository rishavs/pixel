#include "transpiler.h"

void transpile_file(TRANSPILER_CONTEXT* ctx) {
    // add dummy tokens & errors
    // TOKEN t = {TOKEN_IDENTIFIER, "dummy", 0, 0};
    // add_token_to_context(ctx, TOKEN_IDENTIFIER, "dummy", 0, 0);
    // add_error_to_context(ctx, "SyntaxError", "Unexpected Token", "Unexpected Token", "test.c", 0, 0, __FILE__, __LINE__);

    lex_file(ctx);

    // print tokens
    for (size_t i = 0; i < ctx->tokens_count; i++) {
        TOKEN t = ctx->tokens[i];
        printf("Token: %s\n", t.value);
    }

    // add dummy return value
    ctx->cFileCode = "\
int main() {\
    return 0;\
}\
";
    
}