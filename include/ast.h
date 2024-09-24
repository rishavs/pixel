// ast.h

#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct Leaf {
    // enum {
    //     PROGRAM_NODE,
    //     BLOCK_NODE,
    //     FUNC_NODE,
    //     VAR_NODE,
    // } kind;
    char* kind;
    size_t pos;
    size_t line;
    int variant;              // used for conveying any other attributes (eg. var vs const)
    char* value;              // for literals
    Leaf* parent;
    Leaf** args;        // for args, params, and such
    Leaf** statements;      // for blocks
} Leaf;

#endif // AST_H
