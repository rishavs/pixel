
#include "ast.h"
#include "pixel.h"
#include "compiler.h"

Node* parse_expression(List* errors, List* tokens, size_t* i, char* filepath);


Node* parse_integer (List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    (*i)++;
    Node* int_node = (Node*)malloc(sizeof(Node));
    if (!int_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    int_node->type = "INTEGER";
    int_node->Node_Integer.value = token->value;
    int_node->line = token->line;
    int_node->pos = token->pos;

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
    dec_node->Node_Decimal.value = token->value;
    dec_node->line = token->line;
    dec_node->pos = token->pos;

    return dec_node;
};


Node* parse_grouped_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    (*i)++; // Consume '('
    Node* expr = parse_expression(errors, tokens, i, filepath);
    if (!expr) return NULL;
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "RPAREN") != 0) {
        add_error_to_list(errors, "SyntaxError", "Unmatched Parentheses", "Expected a closing parenthesis", filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a closing parenthesis");
        return NULL;
    }
    (*i)++; // Consume ')'
    return expr;
}

Node* parse_primary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "INTEGER") == 0) {
        return parse_integer(errors, tokens, i, filepath);
    } else if (strcmp(token->kind, "DECIMAL") == 0) {
        return parse_decimal(errors, tokens, i, filepath);
    } else if (strcmp(token->kind, "LPAREN") == 0) {
        return parse_grouped_expression(errors, tokens, i, filepath);
    } else {
        add_error_to_list(errors, "SyntaxError", "Invalid Expression", "Expected an expression", filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a primary expression in the primary expression function");
        return NULL;
    }
}

Node* parse_unary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    puts("Parsing unary expression");
    Token* token = tokens->items[*i];
    printf("Parsing unary expression for token %s\n", token->value);
    if (
        strcmp(token->kind, "MINUS") == 0 || strcmp(token->kind, "NOT") == 0
    ) {
        (*i)++;
        Node* unr_expr = parse_primary_expression(errors, tokens, i, filepath);
        if (!unr_expr) {
            perror("Failed to parse primary expression in the unary function");
            return NULL;
        }
        Node* unary_node = (Node*)malloc(sizeof(Node));
        if (!unary_node) {
            perror("Failed to allocate memory for unary expression node");
            exit(EXIT_FAILURE);
        }
        unary_node->type = "UNARY_EXPRESSION";
        unary_node->line = token->line;
        unary_node->pos = token->pos;
        unary_node->Node_Unary.right = unr_expr;
        unary_node->Node_Unary.operator = token->value;

        return unary_node;
    } else {
        Node* expr = parse_primary_expression(errors, tokens, i, filepath);
        if (!expr) {
            perror("Failed to parse primary expression in the else branch of unary function");
            return NULL;
        }
        return expr;
    }
}

Node* parse_binary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    Node* left = parse_unary_expression(errors, tokens, i, filepath);
    if (!left) {
        perror("Failed to parse the left unary expression in binary expression");
        return NULL;
    }
    if (*i >= tokens->length) return left;

    Token* op_token = tokens->items[*i];
    if (
        strcmp(op_token->kind, "PLUS") == 0     || strcmp(op_token->kind, "MINUS") == 0 ||
        strcmp(op_token->kind, "MULTIPLY") == 0 || strcmp(op_token->kind, "DIVIDE") == 0
    ) {
        (*i)++;
        Node* right = parse_unary_expression(errors, tokens, i, filepath);
        if (!right) {
            perror("Failed to parse unary expression in binary expression");
            return NULL;
        }

        Node* binary_node = (Node*)malloc(sizeof(Node));
        if (!binary_node) {
            perror("Failed to allocate memory for binary expression node");
            exit(EXIT_FAILURE);
        }
        char* first_operator = op_token->value;
        
        binary_node->type = "BINARY_EXPRESSION";
        binary_node->line = op_token->line;
        binary_node->pos = op_token->pos;
        binary_node->Node_Binary.left = left;
        binary_node->Node_Binary.right = right;
        binary_node->Node_Binary.operator = first_operator;
        // Check for additional binary operators
        while (*i < tokens->length) {
            Token* next_op = tokens->items[*i];
            if (strcmp(next_op->kind, op_token->kind) == 0) {
                (*i)++;
                Node* next_right = parse_unary_expression(errors, tokens, i, filepath);
                if (!next_right) {
                    perror("Failed to parse unary expression in the while loop of the binary expression");
                    return NULL;
                }

                Node* new_binary = (Node*)malloc(sizeof(Node));
                if (!new_binary) {
                    perror("Failed to allocate memory for binary expression node");
                    exit(EXIT_FAILURE);
                }

                char* next_operator = next_op->value;
              
                if (strcmp(first_operator, next_operator) != 0) {
                    add_error_to_list(errors, "SyntaxError", "Mixed Binary Operators", "Cannot mix different binary operators without parentheses", filepath, next_op->line, next_op->pos, __FILE__, __LINE__);
                    perror("Cannot mix different binary operators without parentheses");
                    return NULL;
                }
                
                new_binary->type = "BINARY_EXPRESSION";
                new_binary->line = next_op->line;
                new_binary->pos = next_op->pos;
                new_binary->Node_Binary.left = binary_node;
                new_binary->Node_Binary.right = next_right;
                new_binary->Node_Binary.operator = next_operator;

                binary_node = new_binary;
            } else {
                break;
            }
        }

        return binary_node;
    }

    return left;
}


Node* parse_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    printf("Parsing expression\n");
    if (*i >= tokens->length) {
        size_t lastLine = ((Token*)tokens->items[tokens->length - 1])->line;
        size_t lastPos = ((Token*)tokens->items[tokens->length - 1])->pos;
        add_error_to_list(errors, "SyntaxError", "Incomplete Expression", "Expected an expression", filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression");
        return NULL;
    }
    Node* binary_expr = parse_binary_expression(errors, tokens, i, filepath);
    printf("Parsed expression %s\n", binary_expr->type);

    return binary_expr;
}


Node* parse_return(List* errors, List* tokens, size_t* i, char* filepath) {
    (*i)++;  // Consume 'return'

    printf("Parsing return statement\n");
    if (*i >= tokens->length) {
        size_t lastLine = ((Token*)tokens->items[tokens->length - 1])->line;
        size_t lastPos = ((Token*)tokens->items[tokens->length - 1])->pos;
        
        add_error_to_list(errors, "SyntaxError", "Incomplete Return Statement", "Expected an expression after the 'return' statement", filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression after the 'return' statement");
        return NULL;
    }

    Node* expr_node = parse_expression(errors, tokens, i, filepath);
    printf("Parsed expression %s\n", expr_node->type);
    if (!expr_node) {
        perror("Failed to parse expression in return statement");
        return NULL;
    }
    
    Node* ret_node = (Node*)malloc(sizeof(Node));
    if (!ret_node) {
        perror("Failed to allocate memory for return statement");
        exit(EXIT_FAILURE);
    }
    printf("Allocated memory for return statement\n");
    ret_node->type = "RETURN";
    ret_node->line = expr_node->line;
    ret_node->pos = expr_node->pos;
    ret_node->Node_Return.expr = expr_node;

    printf("Return statement: %s\n", ret_node->type);
    return ret_node;
}

List* parse_block(List* errors, List* tokens, size_t* i, char* filepath) {
    List* block = list_init("List<Node>");

    bool in_recovery_loop = false;

    while (*i < tokens->length) {
        Token* token = tokens->items[*i];

        if (strcmp(token->kind, "RETURN") == 0) {
            in_recovery_loop = false;
            puts("Parsing return statement");
            Node* ret_node = parse_return(errors, tokens, i, filepath);
            printf("Parsed return statement %s", ret_node->type);
            if (!ret_node) {
                in_recovery_loop = true;
                printf("Failed to parse return statement\n");
                return block;
            }
            list_push(block, ret_node);

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
                perror(err_msg);
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

    List* statements = parse_block(errors, tokens, &i, filepath);
    printf("\nStatements: %zu\n", statements->length);
    program->Node_Program.block = statements;

    return program;
}