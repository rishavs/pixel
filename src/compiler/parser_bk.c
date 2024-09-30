
#include "ast.h"
#include "pixel.h"
#include "compiler.h"

// Get the precedence of an operator
int get_precedence(char* type) {
    if      (strcmp(type, "EQUALS") == 0)       return 1;
    else if (strcmp(type, "NOT_EQUALS") == 0)   return 1;
    else if (strcmp(type, "MINUS") == 0)        return 2;
    else if (strcmp(type, "PLUS") == 0)         return 2;
    else if (strcmp(type, "MULTIPLY") == 0)     return 3;
    else if (strcmp(type, "DIVIDE") == 0)       return 3;
    else                                        return 0;
    
}

Node* parse_integer (List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    (*i)++;
    Node* int_node = (Node*)malloc(sizeof(Node));
    if (!int_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    int_node->type = "INTEGER";
    int_node->Node_Number.value = token->value;
    int_node->line = token->line;
    int_node->pos = token->pos;
    int_node->Node_Number.value = token->value;

    return int_node;
};
Node* parse_decimal (List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    (*i)++;
    Node* dec_node = (Node*)malloc(sizeof(Node));
    if (!dec_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    dec_node->type = "DECIMAL";
    dec_node->Node_Number.value = token->value;
    dec_node->line = token->line;
    dec_node->pos = token->pos;
    dec_node->Node_Number.value = token->value;

    return dec_node;
};

Node* parse_primary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "INT") == 0) {
        return parse_integer(errors, tokens, i, filepath);
    } else if (strcmp(token->kind, "DECIMAL") == 0) {
        return parse_decimal(errors, tokens, i, filepath);
    } else {
        size_t lastLine = token->line;
        size_t lastPos = token->pos;
        
        add_error_to_list(errors, "SyntaxError", "Invalid Expression", "Expected an expression", filepath, lastLine, lastPos, __FILE__, __LINE__);
        
        return NULL;
    }
}

Node* parse_unary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "MINUS") == 0) {
        (*i)++;
        Node* operand = parse_expression(errors, tokens, i, filepath);
        if (!operand) {
            return NULL;
        }

        Node* unary_node = (Node*)malloc(sizeof(Node));
        if (!unary_node) {
            perror("Failed to allocate memory for unary expression node");
            exit(EXIT_FAILURE);
        }
        unary_node->type = "UNARY_MINUS";
        unary_node->value = NULL;
        unary_node->line = token->line;
        unary_node->pos = token->pos;
        unary_node->expr = operand;

        return unary_node;
    } else {
        size_t lastLine = token->line;
        size_t lastPos = token->pos;
        
        add_error_to_list(errors, "SyntaxError", "Invalid Expression", "Expected an expression", filepath, lastLine, lastPos, __FILE__, __LINE__);
        
        return NULL;
    }
}

Node* parse_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    if (*i >= tokens->length) {
        size_t lastLine = ((Token*)tokens->items[tokens->length - 1])->line;
        size_t lastPos = ((Token*)tokens->items[tokens->length - 1])->pos;
        
        add_error_to_list(errors, "SyntaxError", "Incomplete Expression", "Expected an expression", filepath, lastLine, lastPos, __FILE__, __LINE__);
        
        return NULL;
    }
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "INT") == 0) {
        (*i)++;
        Node* int_node = parse_integer(errors, tokens, i, filepath);
        return int_node;
    // } else if (strcmp(token->kind, "MINUS") == 0) {
    //     (*i)++;
    //     Node* operand = parse_expression(errors, tokens, i, filepath);
    //     if (!operand) {
    //         return NULL;
    //     }

    //     Node* unary_node = (Node*)malloc(sizeof(Node));
    //     if (!unary_node) {
    //         perror("Failed to allocate memory for unary expression node");
    //         exit(EXIT_FAILURE);
    //     }
    //     unary_node->type = "UNARY_MINUS";
    //     unary_node->value = NULL;
    //     unary_node->line = token->line;
    //     unary_node->pos = token->pos;
    //     unary_node->expr = operand;

    //     return unary_node;
    } else {
        size_t lastLine = token->line;
        size_t lastPos = token->pos;
        
        add_error_to_list(errors, "SyntaxError", "Invalid Expression", "Expected an expression", filepath, lastLine, lastPos, __FILE__, __LINE__);
        
        return NULL;
    }
}


Node* parse_return(List* errors, List* tokens, size_t* i, char* filepath) {
    if (*i >= tokens->length) {
        size_t lastLine = ((Token*)tokens->items[tokens->length - 1])->line;
        size_t lastPos = ((Token*)tokens->items[tokens->length - 1])->pos;
        
        add_error_to_list(errors, "SyntaxError", "Incomplete Return Statement", "Expected an expression after the 'return' statement", filepath, lastLine, lastPos, __FILE__, __LINE__);
        
        return NULL;
    }

    Node* expr_node = parse_expression(errors, tokens, i, filepath);
    if (!expr_node) {
        return NULL;
    }
    
    Node* ret_node = (Node*)malloc(sizeof(Node));
    if (!ret_node) {
        perror("Failed to allocate memory for return statement");
        exit(EXIT_FAILURE);
    }
    ret_node->type = "RETURN";
    ret_node->line = expr_node->line;
    ret_node->pos = expr_node->pos;
    ret_node->Node_Return.expr = expr_node;

    return ret_node;
}

List* parse_block(List* errors, List* tokens, size_t* i, char* filepath) {
    List* block = list_init("List<Node>");

    bool in_recovery_loop = false;

    while (*i < tokens->length) {
        Token* token = tokens->items[*i];

        if (strcmp(token->kind, "RETURN") == 0) {
            (*i)++;
            in_recovery_loop = false;

            Node* rest_node = parse_return(errors, tokens, i, filepath);
            if (!rest_node) {
                return NULL;
            }
            list_push(block, rest_node);

        } else {
            if (!in_recovery_loop) {
                in_recovery_loop = true;
             
                char* msg = "Expected a statement, but instead found: ";
                size_t msg_len = strlen(msg) + strlen(token->value) + 1;
                char* err_msg = (char*)malloc(msg_len);
                if (!err_msg) {
                    perror("Failed to allocate memory for error message");
                    exit(EXIT_FAILURE);
                }
                snprintf(err_msg, msg_len, "%s%s", msg, token->value);
                add_error_to_list(errors, "SyntaxError", "Unexpected Token", err_msg,  filepath, token->line, token->pos, __FILE__, __LINE__);
            }
        }
        (*i)++;
    }
    return block;
}

Node* parse_file(List* errors, List* tokens, char* filepath) {

    size_t i = 0;

    Node* program = (Node*)malloc(sizeof(Node));
    if (!program) {
        perror("Failed to allocate memory for file node");
        exit(EXIT_FAILURE);
    }
    program->type = "PROGRAM";
    program->line = 0;
    program->pos = 0;
    program->Node_Program.filepath = filepath;
    program->Node_Program.block = parse_block(errors, tokens, &i, filepath);

    return program;
}