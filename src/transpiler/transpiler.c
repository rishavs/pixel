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
        printf("%s: %s\n", list_of_token_kinds[t.kind], t.value);
    }

    // print errors
    for (size_t i = 0; i < ctx->errors_count; i++) {
        TRANSPILER_ERROR e = ctx->errors[i];
        printf("%s: %s: %s\n", e.category, e.header, e.msg);
    }

    // parse_file(ctx);

    // // check if the parser initialized correctly
    // if (!ctx->root) {
    //     perror("Failed to parse the file");
    //     return;
    // }

    // add dummy return value
    ctx->cFileCode = "\
int main() {\
    return 0;\
}\
";
    
}