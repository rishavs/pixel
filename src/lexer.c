#include "compiler.h"


Token* lex_file (char* src, char* filepath) {
    // Create a list of tokens. Its size can the size of the source code for simplicity.
    Token* tokens = malloc(sizeof(Token) * strlen(src));

    // Initialize the lexer
    size_t line = 1;
    size_t pos = 0;
    size_t token_count = 0;

    // Initialize the iterator
    size_t str_len = strlen(src);
    char c = src[0];

    // Loop through the source code
    while (pos < str_len && c != '\0') {
        c = src[pos];

        Token t;
        t.kind = TOKEN_ILLEGAL;
        t.value = c;
        t.pos = pos;
        t.line = line;

        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\r') {
            pos++;
        } else if (c == '\n') {
            pos++;
            line++;

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
                    break;
                }
                pos++;
            }
            if (!comment_closed) {
                printf("Error: Unclosed multi-line comment at line %zu, pos %zu\n", line, pos);
                return NULL;
            }

        // Handle numbers
        } else if (c >= '0' && c <= '9') {
            t.kind = TOKEN_INT;
            while (c >= '0' && c <= '9' && pos < str_len && c != '\0') {
                c = src[++pos];
            }



    }

}








// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>

// #include "compiler.h"

// ListOfTokens lex_file (cstr src) {
//     ListOfTokens tokens = {0};
    
//     size_t line = 1;

//     cstr_iter i = cstr_begin(&src);
//     while (i.ref != cstr_end(&src).ref && i.ref != NULL) {
//         Token t;
//         t.kind = cstr_init();
//         t.value = cstr_init();
//         t.pos = 0;
//         t.line = line;

//         // skip whitespace
//         if (*i.ref == ' ' || *i.ref == '\t' || *i.ref == '\r') {
//             i = cstr_advance(i, 1);
//             continue;
//         } else if (*i.ref == '\n') {
//             i = cstr_advance(i, 1);
//             line++;
//             continue;

//         // handle identifiers and keywords
//         } else if (*i.ref >= 'a' && *i.ref <= 'z' || *i.ref >= 'A' && *i.ref <= 'Z') {
//             t.pos = cstr_topos(&src, i);

//             while (
//                 *i.ref >= 'a' && *i.ref <= 'z' 
//                 || *i.ref >= 'A' && *i.ref <= 'Z'
//                 || *i.ref >= '0' && *i.ref <= '9' 
//                 || *i.ref == '_'
//             ) {
//                 cstr_push(&t.value, i.ref);
//                 i = cstr_advance(i, 1);
//             }
//             if (cstr_equals(&t.value, "let")) {
//                 t.kind = cstr_lit("LET");
//             } else if (cstr_equals(&t.value, "mut")) {
//                 t.kind = cstr_lit("MUT");
//             } else {
//                 t.kind = cstr_lit("ID");
//             }

//         // handle numbers
//         } else if (*i.ref >= '0' && *i.ref <= '9') {
//             t.pos = cstr_topos(&src, i);

//             bool is_float = false;
//             while (
//                 *i.ref >= '0' && *i.ref <= '9'
//                 || *i.ref == '.' || *i.ref == 'e'
//                 || *i.ref == '_'
//             ) {
//                 if (*i.ref == '_') {
//                     i = cstr_advance(i, 1);
//                 }
//                 if (*i.ref == '.') {
//                     is_float = true;
//                 }
//                 cstr_push(&t.value, i.ref);
//                 i = cstr_advance(i, 1);
//             }
//             if (is_float) {
//                 t.kind = cstr_lit("FLOAT");
//             } else {
//                 t.kind = cstr_lit("INT");
//             }

//         // handle operators
//         } else if (*i.ref == '=') {
//             t.pos = cstr_topos(&src, i);

//             cstr_push(&t.value, i.ref);
//             i = cstr_advance(i, 1);
//             if (*i.ref == '=') {
//                 t.kind = cstr_lit("EQ");
//                 cstr_push(&t.value, i.ref);
//                 i = cstr_advance(i, 1);
//             } else {
//                 t.kind = cstr_lit("ASSIGN");
//             }
            
//         // handle illegals
//         } else {
//             t.pos = cstr_topos(&src, i);
            
//             t.kind = cstr_lit("ILLEGAL");
//             cstr_push(&t.value, i.ref);
//             i = cstr_advance(i, 1);

//         }

//         // push the token to the list
//         t.line = line;
//         ListOfTokens_push(&tokens, t);        
//     }
   
//     return tokens;
// }