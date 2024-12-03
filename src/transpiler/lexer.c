#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "errors.h"
#include "helpers.h"
#include "transpiler.h"

void lex_file(TRANSPILER_CONTEXT* ctx) {

    // Initialize the iterator
    ctx->src_len = strlen(ctx->src);
    char c = ctx->src[0];

    // Loop through the source code
    size_t pos = 0;
    size_t line = 0;
    while (c != '\0') {
        c = ctx->src[pos];

        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            while (pos < ctx->src_len && c != '\0') {
                c = ctx->src[pos];
                if (c == ' ' || c == '\t' || c == '\r') {
                    pos++;
                } else if (c == '\n') {
                    pos++;
                    line++;
                } else {
                    break;
                }
            }

        // Skip single line comments
        } else if (c == '-' && ctx->src[pos + 1] == '-') {
            while (pos < ctx->src_len && c != '\0') {
                c = ctx->src[pos];

                if (c == '\n') {
                    line++;
                    break;
                }
                pos++;
            }

        // Skip multi-line comments
        } else if (c == '-' && ctx->src[pos + 1] == '[') {
            bool comment_closed = false;
            
            while (pos < ctx->src_len && c != '\0') {
                c = ctx->src[pos];
                if (c == '\n') {
                    line++;
                }
                if (c == ']' && ctx->src[pos + 1] == '-') {
                    pos += 2;
                    comment_closed = true;
                    break;
                }
                pos++;
            }
            if (!comment_closed) {
                // add_error_to_list(errors, "SyntaxError", "Unclosed multi-line comment", "Unclosed multi-line comment", filepath, line, pos, __FILE__, __LINE__);
                add_error_to_context(ctx, "SyntaxError", "Unclosed multi-line comment", "Unclosed multi-line comment", ctx->filepath, line, pos, __FILE__, __LINE__);
                return;
            }

        // Handle operators and symbols
        } else if (string_starts_with(ctx->src, ctx->src_len, pos, "=")) {
            add_token_to_context(ctx, TOKEN_ASSIGN, "=", pos, line);
            pos++;
        
        } else if (string_starts_with(ctx->src, ctx->src_len, pos, "(")) {
            add_token_to_context(ctx, TOKEN_LPAREN, "(", pos, line);
            pos++;

        } else if (string_starts_with(ctx->src, ctx->src_len, pos, ")")) {
            add_token_to_context(ctx, TOKEN_RPAREN, ")", pos, line);
            pos++;

        // Handle identifiers
        } else if (
            (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') ||
            (c == '_')
        ) {
            size_t anchor = pos;

            while (pos < ctx->src_len && c != '\0') {
                c = ctx->src[pos];
               if (
                    (c >= 'a' && c <= 'z') || 
                    (c >= 'A' && c <= 'Z') ||
                    (c == '_') ||
                    (c >= '0' && c <= '9')
                ) {
                    pos++; 
                } else {
                    break;
                }
            }

            char* buffer;
            buffer = calloc(pos - anchor + 1, sizeof(char));
            if (buffer == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
            strncpy(buffer, ctx->src + anchor, pos - anchor);
            buffer[pos - anchor] = '\0';

            if (strcmp(buffer, "return") == 0) {
                add_token_to_context(ctx, TOKEN_RETURN, "return", anchor, line);
            } else if (strcmp(buffer, "let") == 0) {
                add_token_to_context(ctx, TOKEN_LET, "let", anchor, line);
            } else if (strcmp(buffer, "var") == 0) {
                add_token_to_context(ctx, TOKEN_VAR, "var", anchor, line);
            } else {
                add_token_to_context(ctx, TOKEN_IDENTIFIER, buffer, anchor, line);
            }
            // free(buffer);

        // Handle numbers
        } else if (c >= '0' && c <= '9') {
            TOKENKIND number_kind = TOKEN_INTEGER;
            size_t anchor = pos;

            while (pos < ctx->src_len && c != '\0') {
                c = ctx->src[pos];

                if ((c >= '0' && c <= '9') || c == '.' || c == '_') {
                    if (c == '_') {
                        pos++;
                        continue;
                    }
                    if (c == '.') {
                        if (number_kind == TOKEN_INTEGER) {
                            number_kind = TOKEN_DECIMAL;
                        } else {
                            break;
                        }

                    }
                } else {
                    break;
                }
                pos++;
            }
         
            char* buffer;
            buffer = calloc(pos - anchor + 1, sizeof(char));
            if (buffer == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
            strncpy(buffer, ctx->src + anchor, pos - anchor);
            buffer[pos - anchor] = '\0';

            add_token_to_context(ctx, number_kind, buffer, anchor, line);


        // Handle illegal characters
        } else {
            char* illegal_str = calloc(2, sizeof(char));
            if (illegal_str == NULL) fatal_memory_allocation_failure(__FILE__, __LINE__);
            illegal_str[0] = c;
            illegal_str[1] = '\0';
            add_error_to_context(ctx, "SyntaxError", "Illegal character in source", illegal_str, ctx->filepath, line, pos, __FILE__, __LINE__);
            
            pos++;
        }
    }
}