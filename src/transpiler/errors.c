#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resources.h"
#include "helpers.h"
#include "transpiler.h"

// Memory allocaltion failed. Fatal failure.
void memory_allocation_failure(size_t pos, size_t line, const char* filepath, char* transpiler_file, size_t transpiler_line) {
    fprintf(stderr, "\033[0;31m%s %s ", en_us[RES_MEMORY_FAILURE_CAT], en_us[RES_MEMORY_FAILURE_MSG]);
    if (pos && line && filepath) { // memory failures need not be tied to user code
        fprintf(stderr, en_us[RES_PROBLEM_AT], pos, line, filepath);
    };   
    perror(en_us[RES_INTERNAL_PERROR]);
    fprintf(stderr, en_us[RES_INTERNAL_LOCATION], transpiler_line, transpiler_file);
    fprintf(stderr, "\n\033[0m");
    exit(EXIT_FAILURE);
}

void unhandled_error(size_t pos, size_t line, const char* filepath, char* transpiler_file, size_t transpiler_line){
    fprintf(stderr, "\033[0;31m%s %s", en_us[RES_UNHANDLED_ERROR_CAT], en_us[RES_UNHANDLED_ERROR_MSG]);
    fprintf(stderr, en_us[RES_PROBLEM_AT], pos, line, filepath);
    fprintf(stderr, "\n%s\n", en_us[RES_UNHANDLED_ERROR_NOTE]);
    perror(en_us[RES_INTERNAL_PERROR]);
    fprintf(stderr, en_us[RES_INTERNAL_LOCATION], transpiler_line, transpiler_file);
    fprintf(stderr, "\n\033[0m");
    exit(EXIT_FAILURE);
}

void add_error_to_context(Transpiler_context_t* ctx, const char* category, const char* msg, const size_t pos, const size_t line, const char* transpiler_file, const size_t transpiler_line) {
    if (ctx->errors_count >= ctx->errors_capacity - 1) {
        ctx->errors_capacity *= 2;
        ctx->errors = realloc(ctx->errors, ctx->errors_capacity * sizeof(Transpiler_error_t));
        if (ctx->errors == NULL) memory_allocation_failure(pos, line, ctx->filepath, __FILE__, __LINE__);
    }

    // TODO - handle very long file names? dynamically allocate memory?
    ctx->errors[ctx->errors_count].category = strdup(category);
    ctx->errors[ctx->errors_count].msg = strdup(msg);

    ctx->errors[ctx->errors_count].pos = pos;
    ctx->errors[ctx->errors_count].line = line;
    ctx->errors[ctx->errors_count].filepath = strdup(ctx->filepath);
    ctx->errors[ctx->errors_count].transpiler_file = strdup(transpiler_file);
    ctx->errors[ctx->errors_count].transpiler_line = transpiler_line;

    ctx->errors_count++;
}

void parser_expected_syntax_error(Transpiler_context_t* ctx, const char* expected_syntax, const char* transpiler_file, const size_t transpiler_line) {
    Token_t token = ctx->tokens[ctx->i];
    char* err_msg;
    if (ctx->i >= ctx->tokens_count) {
        err_msg = join_strings(3,
            build_string(en_us[RES_EXPECTED], expected_syntax), 
            " ", 
            en_us[RES_REACHED_EOS]
        );
    } else {
        err_msg = join_strings(3,
            build_string(en_us[RES_EXPECTED], expected_syntax), 
            " ", 
            build_string(en_us[RES_FOUND_X], get_substring(ctx->src, token.pos, token.len)));
    }
    add_error_to_context(ctx, 
        en_us[RES_SYNTAX_ERROR_CAT], 
        err_msg, 
        token.pos, token.line, __FILE__, __LINE__
    );
   
}


// --------------------------------------
// Lexer Errors
// --------------------------------------
// void illegal_char_error(Transpiler_context_t* ctx, const char c, const size_t pos, const size_t line, const char* transpiler_file, const size_t transpiler_line) {
//     add_error_to_context(ctx,
//         en_us[RES_SYNTAX_ERROR_CAT],
//         en_us[RES_ILLEGAL_CHAR_MSG],
//         pos, line, transpiler_file, transpiler_line
//     );
// }

// void unclosed_delimiter_error(Transpiler_context_t* ctx, char* delim, size_t pos, size_t line, char* transpiler_file, size_t transpiler_line) {
//     add_error_to_context(ctx,
//         en_us[RES_SYNTAX_ERROR_CAT],
//         sprintf("Found an unclosed delimiter \"%s\", ", delim),
//         pos, line, transpiler_file, transpiler_line
//     );
// }

// --------------------------------------
// Parser Errors
// --------------------------------------
// void missing_syntax_error(Transpiler_context_t* ctx, const char* expected_syntax, const size_t found_len, const size_t pos, size_t line, const char* transpiler_file, const size_t transpiler_line) {
//     char* found = calloc (found_len + 1, sizeof(char));
//     if (found == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);
//     strncpy(found, ctx->src + pos, found_len);
//     found[found_len] = '\0';

//     add_error_to_context(ctx,
//         "SyntaxError",
//         sprintf("Expected %s, but instead found \"%s\", ", expected_syntax, found ),
//         pos, line, transpiler_file, transpiler_line
//     );
// }

// void missing_token_error(Transpiler_context_t* ctx, const char* expected_token, const char* expected_syntax, const size_t found_len, const size_t pos, size_t line, const char* transpiler_file, const size_t transpiler_line) {
//     char* found = calloc (found_len + 1, sizeof(char));
//     if (found == NULL) memory_allocation_failure(0, 0, ctx->filepath, __FILE__, __LINE__);
//     strncpy(found, ctx->src + pos, found_len);
//     found[found_len] = '\0';

//     add_error_to_context(ctx,
//         "SyntaxError",
//         sprintf("Expected \"%s\" for %s, but instead found \"%s\", ", expected_token, expected_syntax, found ),
//         pos, line, transpiler_file, transpiler_line
//     );
// }

// --------------------------------------
// Symbol building Errors
// --------------------------------------
// void missing_symbol_initialization_error(const char* symbol, const char* filepath, size_t at, size_t line) {
//     fprintf(stderr, "\033[0;31m[ ERROR ] "); // Red color for Errors
//     fprintf(stderr, "Missing initialization for the symbol \"%s\" at %zu:%zu in %s\n", symbol, line, at, filepath);
// }
// void symbol_redeclaration_error(const char* symbol, const char* filepath, size_t at, size_t line) {
//     fprintf(stderr, "\033[0;31m[ ERROR ] "); // Red color for Errors
//     fprintf(stderr, "Previously declared Symbol \"%s\" is being redeclared at %zu:%zu in %s\n", symbol, line, at, filepath);
// }
