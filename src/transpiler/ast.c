

create_ast_node* create_ast_node(const char* filepath) {
    create_ast_node* ctx = (create_ast_node*)malloc(sizeof(create_ast_node));
    if (ctx == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    ctx->filepath = strdup(filepath);

    // Initialize error handling
    ctx->errors_count = 0;
    ctx->errors_capacity = 8; // Initial capacity for errors
    ctx->errors = calloc(ctx->errors_capacity, sizeof(create_ast_error));
    if (ctx->errors == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);

    // Initialize cursor
    ctx->i = 0;

    // Initialize lexer
    ctx->tokens_count = 0;
    ctx->tokens_capacity = 8; // Initial capacity for tokens
    ctx->tokens = calloc(ctx->tokens_capacity, sizeof(TOKEN));
    if (ctx->tokens == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
    ctx->lexing_duration = 0.0;

    // add default c code
    ctx->cFileCode = DEFAULT_CFILE_CODE;
    ctx->hFileCode = "";

    return ctx;
}