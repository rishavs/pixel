#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool lex_file(List* errors, List* tokens, char* src, char* filepath) {
    // Initialize the lexer
    bool ok = true;
    size_t line = 0;
    size_t pos = 0;
    size_t token_count = 0;

    // Initialize the iterator
    size_t str_len = strlen(src);
    char c = src[0];

    // Loop through the source code
    while (pos < str_len && c != '\0') {
        c = src[pos];

        Token* t = (Token*)malloc(sizeof(Token));
        if (!t) {
            perror("Memory allocation failed for token");
            return NULL;
        }
        t->kind = TOKEN_ILLEGAL;
        t->value = NULL;
        t->pos = pos;
        t->line = line;

        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            while (pos < str_len && c != '\0') {
                c = src[pos];
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
        } else if (c == '-' && src[pos + 1] == '-') {
            while (pos < str_len && c != '\0') {
                c = src[pos];

                if (c == '\n') {
                    line++;
                    break;
                }
                pos++;
            }

        // Skip multi-line comments
        } else if (c == '-' && src[pos + 1] == '[') {
            bool comment_closed = false;
            
            while (pos < str_len && c != '\0') {
                c = src[pos];
                if (c == '\n') {
                    line++;
                }
                if (c == ']' && src[pos + 1] == '-') {
                    pos += 2;
                    comment_closed = true;
                    break;
                }
                pos++;
            }
            if (!comment_closed) {
                add_error_to_list(errors, "SyntaxError", "Unclosed multi-line comment", "Unclosed multi-line comment", filepath, line, pos, __FILE__, __LINE__);
                return NULL;
            }

        // Handle numbers
        } else if (c >= '0' && c <= '9') {
            t->kind = TOKEN_INT;
            size_t start_pos = pos;
            while (pos < str_len && c != '\0') {
                c = src[pos];

                if ((c >= '0' && c <= '9') || c == '.' || c == '_') {
                    if (c == '_') {
                        pos++;
                        continue;
                    }
                    if (c == '.') {
                        if (t->kind == TOKEN_INT) {
                            t->kind = TOKEN_DECIMAL;
                        } else {
                            break;
                        }
                    }
                } else {
                    break;
                }
                pos++;
            }
            size_t length = pos - start_pos;
            t->value = (char*)malloc(length + 1);
            if (!t->value) {
                perror("Memory allocation failed for token value");
                free(t);
                return NULL;
            }
            strncpy(t->value, src + start_pos, length);
            t->value[length] = '\0';
            
            // Push the token to the list
            list_push(tokens, t);
            token_count++;

        // Handle illegal characters
        } else {
            t->kind = TOKEN_ILLEGAL;
            char msg[64];
            snprintf(msg, sizeof(msg), "Found illegal character '%c'", c);
            
            add_error_to_list(errors, "SyntaxError", "Illegal character in source", msg, filepath, line, pos, __FILE__, __LINE__);
            pos++;
            ok = false;
            
            // Push the token to the list
            list_push(tokens, t);
            token_count++;
        }
    }

    return ok;
}