#ifndef PIXEL_COMPILER_H
#define PIXEL_COMPILER_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "pixel.h"
#include "ast.h"

typedef struct CompilerError {
    char* category;
    char* header;
    char* msg;

    char* pixel_file;
    size_t pixel_line;
    size_t pixel_pos;

    char* c_file;
    size_t c_line;
} CompilerError;

void compiler_error_print(CompilerError* error);
void add_error_to_list(List* CompilerErrors, char* category, char* header, char* msg, char* pixel_file, size_t pixel_line, size_t pixel_pos, char* c_file, size_t c_line);

// Define the macro for the enum and string array
#define TOKEN_KINDS     \
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
    TOKEN_KINDS
    #undef X
} TokenKind;

// Generate the string array using the macro
static const char* TokenKindStrings[] = {
    #define X(token) #token,
    TOKEN_KINDS
    #undef X
};

// Token structure
typedef struct Token {
    TokenKind kind;
    char* value;
    size_t pos;
    size_t line;
} Token;

// Define the macro for the enum and string array
#define NODE_KINDS      \
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
    NODE_KINDS
    #undef X
} NodeKind;

// Generate the string array using the macro
static const char* NodeKindStrings[] = {
    #define X(node) #node,
    NODE_KINDS
    #undef X
};

// Node structure
typedef struct Node Node;
struct Node {
    NodeKind    kind;
    size_t      pos;
    size_t      line;
    int         depth;

    Node*       parent;
    Node*       scope_owner;
    
    union {
        struct { char* value; } Node_Integer;
        struct { char* value; } Node_Decimal;
        struct { char* value; } Node_Identifier;
        struct { char* operator; struct Node *right; } Node_Unary;
        struct { char* operator; List* expressions; } Node_Binary; // can a qualified chain just be binary?
        struct { bool is_var; bool is_new; bool is_assignment; Node* identifier; struct Node* expr; } Node_Declaration;
        struct { struct Node *expr; } Node_Return;
        struct { char* filepath; struct List* block; } Node_Program;
    };
};

void compile_file (char* filepath);
bool read_file (List*, char**, char*);
bool lex_file (List* errors, List* tokens, char* src, char* filepath);

bool parse_file(List* errors, Node* program, List* tokens, char* filepath);
void generate_code (Node* program, char* filepath);


#endif // PIXEL_COMPILER_H