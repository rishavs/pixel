#include <stdlib.h>

#ifndef PIXEL_AST_H
#define PIXEL_AST_H

typedef struct Node Node;
struct Node {
    char* type;
    size_t pos;
    size_t line;
    
    union {
        // Primary
        struct { char* value; } Node_Integer;
        struct { char* value; } Node_Decimal;

        // Unary
        struct { char* operator; struct Node *right; } Node_Unary;
        
        // Binary Expression
        struct { char* operator; struct Node *left; struct Node *right; } Node_Binary;

        // Statements
        struct { struct Node *expr; } Node_Return;

        // Program
        struct { char* filepath; struct List* block; } Node_Program;
    };
};

#endif // PIXEL_AST_H
