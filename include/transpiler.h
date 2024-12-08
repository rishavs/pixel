#ifndef PIXEL_TRANSPILER_H
#define PIXEL_TRANSPILER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"

#define DEFAULT_CFILE_CODE "\
int main() {\
    return 0;\
}"

// pre-declare the context
typedef struct Transpiler_context_t Transpiler_context_t;
typedef struct Node_t Node_t;

typedef struct Transpiler_error_t {
    char* category;
    char* header;
    char* msg;

    char* filepath;
    size_t line;
    size_t pos;

    char* transpiler_file;
    size_t transpiler_line;

} Transpiler_error_t;


// Define the macro for the enum and string array
#define BUILD_TOKEN_KIND\
    X(TOKEN_ILLEGAL)    \
    X(TOKEN_WHITESPACE) \
    X(TOKEN_EOF)        \
    X(TOKEN_ASSIGN)     \
    X(TOKEN_PLUS)       \
    X(TOKEN_MINUS)      \
    X(TOKEN_ASTERISK)   \
    X(TOKEN_FWD_SLASH)  \
    X(TOKEN_NOT)        \
    X(TOKEN_IDENTIFIER) \
    X(TOKEN_INTEGER)    \
    X(TOKEN_DECIMAL)    \
    X(TOKEN_RETURN)     \
    X(TOKEN_LPAREN)     \
    X(TOKEN_RPAREN)     \
    X(TOKEN_LET)        \
    X(TOKEN_CONST)      \
    X(TOKEN_VAR)

// Generate the enum using the macro
typedef enum {
    #define X(token) token,
    BUILD_TOKEN_KIND
    #undef X
} Token_kind;

// Generate the string array using the macro
static const char* list_of_token_kinds[] = {
    #define X(token) #token,
    BUILD_TOKEN_KIND
    #undef X
};

// Token structure
typedef struct Token_t {
    Token_kind kind;
    size_t pos;
    size_t len;
    size_t line;
} Token_t;

void lex_file(Transpiler_context_t* ctx);

// // Define the macro for the enum and string array
// #define BUILD_NODE_KIND \
//     X(NODE_ILLEGAL)     \
//     X(NODE_INTEGER)     \
//     X(NODE_DECIMAL)     \
//     X(NODE_IDENTIFIER)  \
//     X(NODE_UNARY)       \
//     X(NODE_BINARY)      \
//     X(NODE_DECLARATION) \
//     X(NODE_RETURN)      \
//     X(NODE_PROGRAM)

// // Generate the enum using the macro
// typedef enum {
//     #define X(node) node,
//     BUILD_NODE_KIND
//     #undef X
// } Node_kind;

// // Generate the string array using the macro
// static const char* list_of_node_kinds[] = {
//     #define X(node) #node,
//     BUILD_NODE_KIND
//     #undef X
// };

// struct Node {
//     NODE_KIND    kind;
//     size_t      pos;
//     size_t      line;

//     size_t      scope_depth;              // distance from root in terms of scopes
//     size_t      root_distance;           // distance from root in terms of nodes - parent linkages

//     Node*      parent;         // index of the parent node
//     Node*      scope_owner;      // index of the scope owner node
    
//     union {
//         struct { char* msg; } Node_Ilegal;
//         struct { char* value; } Node_Integer;
//         struct { char* value; } Node_Decimal;
//         struct { char* value; } Node_Identifier;
//         // struct { char* operator; size_t right_index; } Node_Unary;
//         // struct { char* operator; size_t expressions[] } Node_Binary; // can a qualified chain just be binary?
//         // struct { bool is_var; bool is_new; bool is_assignment; Node* identifier; struct Node* expr; } Node_Declaration;
//         // struct { struct Node *expr; } Node_Return;
//         struct { Nodes_List statements; } Node_Program;
//     };
// };

// void parse_file(Transpiler_context* ctx);

struct Transpiler_context_t {
    char* filepath;

    // cursor
    size_t i;

    // Error handling
    Transpiler_error_t* errors;
    size_t errors_count;
    size_t errors_capacity;

    // Reading source
    char* src;
    size_t src_len;
    double reading_duration;

    // Lexer
    Token_t* tokens;
    size_t tokens_count;
    size_t tokens_capacity;
    double lexing_duration;

    char* cFileCode;
    char* hFileCode;

    // Parser
    // Node* root;
    // double parsing_duration;

    // // Codegen
    // cFileCode   : string = "";
    // hFileCode   : string = "";
    // currentDepth: number = 0;
    // usesString  : boolean = false;
    // usesInt     : boolean = false;
    // usesFloat   : boolean = false;
    // exitCode    : number = 0;
    // codegenDuration: number = 0;

};

void create_Transpiler_context_t(Transpiler_context_t* ctx, const char* filepath);
void add_error_to_context(Transpiler_context_t* ctx, const char* category, const char* header, const char* msg, const char* filepath, size_t line, size_t pos, const char* transpiler_file, size_t transpiler_line);
void add_token_to_context(Transpiler_context_t* ctx, Token_kind kind, size_t pos, size_t len, size_t line);
void transpile_file(Transpiler_context_t* ctx);

#endif
