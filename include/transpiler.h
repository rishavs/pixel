#ifndef PIXEL_TRANSPILER_H
#define PIXEL_TRANSPILER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
} TOKENKIND;

// Generate the string array using the macro
static const char* TokenKindStrings[] = {
    #define X(token) #token,
    BUILD_TOKEN_KIND
    #undef X
};

// Token structure
typedef struct Token {
    TOKENKIND kind;
    char* value;
    size_t pos;
    size_t line;
} TOKEN;


typedef struct {
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
void add_token_to_context(TRANSPILER_CONTEXT* ctx, TOKENKIND kind, char* value, size_t pos, size_t line);
void add_error_to_context(TRANSPILER_CONTEXT* ctx, char* category, char* header, char* msg, char* filepath, size_t pos, size_t line, char* transpiler_file, size_t transpiler_line);
void transpile_file(TRANSPILER_CONTEXT* ctx);

void lex_file(TRANSPILER_CONTEXT* ctx);

#endif
