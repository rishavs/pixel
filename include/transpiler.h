#ifndef PIXEL_TRANSPILER_H
#define PIXEL_TRANSPILER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_CFILE_CODE "\
int main() {\
    return 0;\
}"

// pre-declare the context
typedef struct Transpiler_context_t Transpiler_context_t;
typedef struct Node_t Node_t;

typedef struct Transpiler_error_t {
    char* category;
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

// Define the macro for the enum and string array
#define BUILD_NODE_KIND \
    X(NODE_ERROR)       \
    X(NODE_INTEGER)     \
    X(NODE_DECIMAL)     \
    X(NODE_IDENTIFIER)  \
    X(NODE_UNARY)       \
    X(NODE_BINARY)      \
    X(NODE_DECLARATION) \
    X(NODE_RETURN)      \
    X(NODE_PROGRAM)

// Generate the enum using the macro
typedef enum {
    #define X(node) node,
    BUILD_NODE_KIND
    #undef X
} Node_kind;

// Generate the string array using the macro
static const char* list_of_node_kinds[] = {
    #define X(node) #node,
    BUILD_NODE_KIND
    #undef X
};

struct Node_t {
    Node_kind   kind;
    size_t      pos;                // starting position in the source code
    size_t      line;               // line number in the source code

    size_t      scope_depth;        // distance from root in terms of scopes
    size_t      root_distance;      // distance from root in terms of nodes - parent linkages

    size_t      parent;             // index of the parent node
    size_t      scope_owner;        // index of the scope owner node
    
    union {
        struct { size_t len; } Integer_data;
        struct { size_t len; } Decimal_data;
        struct { size_t len; } Identifier_data;
        // struct { char* operator; size_t right_index; } Node_Unary;
        // struct { char* operator; size_t expressions[] } Node_Binary; // can a qualified chain just be binary?
        struct { bool is_var; bool is_new; bool is_assignment; size_t identifier_index; size_t expression_index; } Declaration_data;
        // struct { struct Node *expr; } Node_Return;
        struct { size_t* statements; size_t statements_count; size_t statements_capacity;} Program_data;
    };
};

void parse_file(Transpiler_context_t* ctx);

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

    char* c_code;
    char* h_code;

    // Parser
    // Node_t* root;
    Node_t* nodes;
    size_t nodes_count;
    size_t nodes_capacity;
    double parsing_duration;

    // Codegen
    // currentDepth: number = 0;
    // usesString  : boolean = false;
    // usesInt     : boolean = false;
    // usesFloat   : boolean = false;
    // exitCode    : number = 0;
    // codegenDuration: number = 0;

};

void create_Transpiler_context_t(Transpiler_context_t* ctx, const char* filepath);
void add_token_to_context(Transpiler_context_t* ctx, Token_kind kind, size_t pos, size_t len, size_t line);
size_t add_node_to_context(Transpiler_context_t* ctx, Node_kind kind, size_t pos, size_t line);
size_t add_to_indices(size_t* indices_list, size_t index, size_t count, size_t capacity);
void transpile_file(Transpiler_context_t* ctx);

void print_errors (Transpiler_context_t* ctx);

#endif
