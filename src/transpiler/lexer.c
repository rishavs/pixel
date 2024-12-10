#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "resources.h"
#include "errors.h"
#include "helpers.h"
#include "transpiler.h"

void lex_file(Transpiler_context_t* ctx) {

    // Initialize the iterator
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
                // raise unclosed delimiter error
                add_error_to_context(ctx, en_us[RES_SYNTAX_ERROR_CAT], en_us[RES_UNCLOSED_DELIMITER_MSG], pos, line, __FILE__, __LINE__);
                return;
            }

        // Handle operators and symbols
        } else if (string_starts_with(ctx->src, ctx->src_len, pos, "=")) {
            add_token_to_context(ctx, TOKEN_ASSIGN, pos, 1, line);
            pos++;
        
        } else if (string_starts_with(ctx->src, ctx->src_len, pos, "(")) {
            add_token_to_context(ctx, TOKEN_LPAREN, pos, 1, line);
            pos++;

        } else if (string_starts_with(ctx->src, ctx->src_len, pos, ")")) {
            add_token_to_context(ctx, TOKEN_RPAREN, pos, 1, line);
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
            if (buffer == NULL) memory_allocation_failure(pos, line, ctx->filepath, __FILE__, __LINE__);
            strncpy(buffer, ctx->src + anchor, pos - anchor);
            buffer[pos - anchor] = '\0';

            if (strcmp(buffer, "return") == 0) {
                add_token_to_context(ctx, TOKEN_RETURN, anchor, pos - anchor, line);
            } else if (strcmp(buffer, "let") == 0) {
                add_token_to_context(ctx, TOKEN_LET, anchor, pos - anchor, line);
            } else if (strcmp(buffer, "var") == 0) {
                add_token_to_context(ctx, TOKEN_VAR, anchor, pos - anchor, line);
            } else {
                add_token_to_context(ctx, TOKEN_IDENTIFIER, anchor, pos - anchor, line);
            }
            // free(buffer);

        // Handle numbers
        } else if (c >= '0' && c <= '9') {
            Token_kind number_kind = TOKEN_INTEGER;
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
            add_token_to_context(ctx, number_kind, anchor, pos - anchor, line);


        // Handle illegal characters
        } else {
            char* err_msg = calloc(strlen(en_us[RES_ILLEGAL_CHAR_MSG]) + 1, sizeof(char));
            if (err_msg == NULL) memory_allocation_failure(pos, line, ctx->filepath, __FILE__, __LINE__);

            sprintf(err_msg, en_us[RES_ILLEGAL_CHAR_MSG], c);
            add_error_to_context(ctx, en_us[RES_SYNTAX_ERROR_CAT], err_msg, pos, line, __FILE__, __LINE__);
            pos++;
        }
    }
    ctx->src_len = pos;
}