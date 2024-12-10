
#include "resources.h"
#include "errors.h"
#include "transpiler.h"

void transpile_file(Transpiler_context_t* ctx) {
    // add dummy tokens & errors
    // TOKEN t = {TOKEN_IDENTIFIER, "dummy", 0, 0};
    // add_token_to_context(ctx, TOKEN_IDENTIFIER, "dummy", 0, 0);
    // add_error_to_context(ctx, "SyntaxError", "Unexpected Token", "Unexpected Token", "test.c", 0, 0, __FILE__, __LINE__);

    lex_file(ctx);

    // print tokens
    for (size_t i = 0; i < ctx->tokens_count; i++) {
        Token_t t = ctx->tokens[i];
        char* buffer = calloc(t.len + 1, sizeof(char));
        if (buffer == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);
        strncpy(buffer, ctx->src + t.pos, t.len);
        buffer[t.len] = '\0';
        printf("%s: %s\n", list_of_token_kinds[t.kind], buffer);
    }

    // print errors
    for (size_t i = 0; i < ctx->errors_count; i++) {
        Transpiler_error_t e = ctx->errors[i];
        fprintf(stderr, "\033[0;31m%s %s", e.category, e.msg);
        fprintf(stderr, en_us[RES_ERROR_LOCATION], e.pos, e.line, e.filepath);
        fprintf(stderr, "\n");
        perror(en_us[RES_INTERNAL_PERROR]);
        fprintf(stderr, en_us[RES_INTERNAL_LOCATION], e.transpiler_line, e.transpiler_file);
        fprintf(stderr, "\n\033[0m");
    }

    // parse_file(ctx);

    // // check if the parser initialized correctly
    // printf("Nodes count: %zu\n", ctx->nodes_count);
    // for (size_t i = 0; i < ctx->nodes_count; i++) {
    //     Node_t n = ctx->nodes[i];
    //     printf("Node %zu: %s\n", i, list_of_node_kinds[n.kind]);
    // }
    
};