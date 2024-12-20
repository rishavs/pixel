#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "compiler.h"

bool starts_with(char *src, size_t src_len, size_t i,  char* frag) {
    size_t frag_len = strlen(frag);
    if (i + frag_len > src_len) { return false; }
    for (size_t j = 0; j < frag_len; j++) {
        if (src[i + j] != frag[j]) { return false; }
    }
    return true;
}

char* char_to_string(char c) {
    char* str = (char*)malloc(2 * sizeof(char)); // Allocate memory for 2 characters
    if (!str) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    str[0] = c;  // Assign the character
    str[1] = '\0'; // Null terminator
    return str;
}


bool lex_file(List* errors, List* tokens, char* src, char* filepath) {
    // Initialize the lexer
    bool ok = true;
    size_t line = 0;
    size_t pos = 0;

    // Initialize the iterator
    size_t str_len = strlen(src);
    char c = src[0];

    // Loop through the source code
    while (pos < str_len && c != '\0') {
        c = src[pos];

        Token* t = (Token*)malloc(sizeof(Token));
        if (!t) {
            perror("Memory allocation failed for token");
            exit(EXIT_FAILURE);
        }
        t->kind = TOKEN_ILLEGAL;
        t->value = char_to_string(c);
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
                return false;
            }

        // Handle operators and symbols
        } else if (starts_with(src, str_len, pos, "=")) {
            t->kind = TOKEN_ASSIGN;
            t->value = "=";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);
        
        } else if (starts_with(src, str_len, pos, "-")) {
            t->kind = TOKEN_MINUS;
            t->value = "-";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);
        
        } else if (starts_with(src, str_len, pos, "+")) {
            t->kind = TOKEN_PLUS;
            t->value = "+";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);

        } else if (starts_with(src, str_len, pos, "*")) {
            t->kind = TOKEN_ASTERISK;
            t->value = "*";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);
        
        } else if (starts_with(src, str_len, pos, "/")) {
            t->kind = TOKEN_FWD_SLASH;
            t->value = "/";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);

        } else if (starts_with(src, str_len, pos, "(")) {
            t->kind = TOKEN_LPAREN;
            t->value = "(";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);

        } else if (starts_with(src, str_len, pos, ")")) {
            t->kind = TOKEN_RPAREN;
            t->value = ")";
            t->pos = pos;
            t->line = line;
            pos++;
            list_push(tokens, t);

        // Handle identifiers
        } else if (
            (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') ||
            (c == '_')
        ) {
            size_t end = pos;

            while (pos < str_len && c != '\0') {
                c = src[pos];
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
            buffer = calloc(pos - end + 1, sizeof(char));
            if (buffer == NULL) { 
                perror("Memory allocation via calloc failed"); 
                exit(EXIT_FAILURE);
            };
            strncpy(buffer, src + end, pos - end);
            buffer[pos - end] = '\0';

            if (strcmp(buffer, "return") == 0) {
                t->kind = TOKEN_RETURN;
                t->value = buffer;
            } else if (strcmp(buffer, "let") == 0) {
                t->kind = TOKEN_LET;
                t->value = buffer;
            } else if (strcmp(buffer, "var") == 0) {
                t->kind = TOKEN_VAR;
                t->value = buffer;
            } else {
                t->kind = TOKEN_IDENTIFIER;
                t->value = buffer;
            }
            t->pos = end;
            t->line = line;
            list_push(tokens, t);

        // Handle numbers
        } else if (c >= '0' && c <= '9') {
            t->kind = TOKEN_INTEGER;
            size_t start_pos = pos;
            while (pos < str_len && c != '\0') {
                c = src[pos];

                if ((c >= '0' && c <= '9') || c == '.' || c == '_') {
                    if (c == '_') {
                        pos++;
                        continue;
                    }
                    if (c == '.') {
                        if (t->kind == TOKEN_INTEGER) {
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
                exit(EXIT_FAILURE);
            }
            strncpy(t->value, src + start_pos, length);
            t->value[length] = '\0';
            
            // Push the token to the list
            list_push(tokens, t);

        // Handle illegal characters
        } else {
            t->kind = TOKEN_ILLEGAL;
            t->value = char_to_string(c);
            
                // char* msg = "Expected a statement, but instead found: ";
                // size_t msg_len = strlen(msg) + strlen(token->value) + 1;
                // char* err_msg = (char*)malloc(msg_len);
                // if (!err_msg) {
                //     perror("Failed to allocate memory for error message");
                //     return false;
                // }
                // snprintf(err_msg, msg_len, "%s%s", msg, token->value);
                // add_error_to_list(errors, "SyntaxError", "Unexpected Token", err_msg,  filepath, token->line, token->pos, __FILE__, __LINE__);
            
            char* msg = "Found illegal character: ";
            size_t msg_len = strlen(msg) + 2;
            char* err_msg = (char*)malloc(msg_len);
            if (!err_msg) {
                perror("Failed to allocate memory for error message");
                exit(EXIT_FAILURE);
            }
            snprintf(err_msg, msg_len, "%s%c", msg, c);
            add_error_to_list(errors, "SyntaxError", "Illegal character in source", err_msg, filepath, line, pos, __FILE__, __LINE__);
            
            pos++;
            ok = false;
            
            // Push the token to the list
            list_push(tokens, t);
        }
    }

    return ok;
}