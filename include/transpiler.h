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
typedef struct TRANSPILER_CONTEXT TRANSPILER_CONTEXT;
typedef struct Node Node;


typedef struct TRANSPILER_ERROR {
    char* category;
    char* header;
    char* msg;

    char* filepath;
    size_t line;
    size_t pos;

    char* transpiler_file;
    size_t transpiler_line;

} TRANSPILER_ERROR;


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
} TOKEN_KIND;

// Generate the string array using the macro
static const char* list_of_token_kinds[] = {
    #define X(token) #token,
    BUILD_TOKEN_KIND
    #undef X
};

// Token structure
typedef struct Token {
    TOKEN_KIND kind;
    char* value;
    size_t pos;
    size_t line;
} TOKEN;


void lex_file(TRANSPILER_CONTEXT* ctx);


// Define the macro for the enum and string array
#define BUILD_NODE_KIND \
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
} NODE_KIND;

// Generate the string array using the macro
static const char* list_of_node_kinds[] = {
    #define X(node) #node,
    BUILD_NODE_KIND
    #undef X
};

// Node structure
struct Node {
    NODE_KIND    kind;
    size_t      pos;
    size_t      line;

    size_t      depth;              // distance from root in terms of scopes
    size_t      distance;           // distance from root in terms of nodes - parent linkages

    size_t      parent_index;         // index of the parent node
    size_t      scope_owner_index;      // index of the scope owner node
    
    union {
        struct { char* value; } Node_Integer;
        struct { char* value; } Node_Decimal;
        struct { char* value; } Node_Identifier;
        // struct { char* operator; size_t right_index; } Node_Unary;
        // struct { char* operator; size_t expressions[] } Node_Binary; // can a qualified chain just be binary?
        // struct { bool is_var; bool is_new; bool is_assignment; Node* identifier; struct Node* expr; } Node_Declaration;
        // struct { struct Node *expr; } Node_Return;
        struct { char* filepath; size_t* block; } Node_Program;
    };
};

struct TRANSPILER_CONTEXT {
    char* filepath;

    // cursor
    size_t i;

    // Error handling
    size_t errors_count;
    size_t errors_capacity;
    TRANSPILER_ERROR* errors;

    // Reading source
    char* src;
    size_t src_len;

    // Lexer
    size_t tokens_count;
    size_t tokens_capacity;
    TOKEN* tokens;
    double lexing_duration;

    char* cFileCode;
    char* hFileCode;

    
    // // Lexer
    // li: number = 0;
    // line: number = 0;
    // tokens: Token[] = [];
    // lexingDuration: number = 0;

    // // Parser
    // pi: number = 0;
    // root: RootNode = new RootNode(0, 0);
    // parsingDuration: number = 0;

    // // Codegen
    // cFileCode   : string = "";
    // hFileCode   : string = "";
    // currentDepth: number = 0;
    // usesString  : boolean = false;
    // usesInt     : boolean = false;
    // usesFloat   : boolean = false;
    // exitCode    : number = 0;
    // codegenDuration: number = 0;

} TRANSPILER_CONTEXT;

TRANSPILER_CONTEXT* create_transpiler_context(const char* filepath);
void add_token_to_context(TRANSPILER_CONTEXT* ctx, TOKEN_KIND kind, char* value, size_t pos, size_t line);
void add_error_to_context(TRANSPILER_CONTEXT* ctx, char* category, char* header, char* msg, char* filepath, size_t pos, size_t line, char* transpiler_file, size_t transpiler_line);
void transpile_file(TRANSPILER_CONTEXT* ctx);


#endif
