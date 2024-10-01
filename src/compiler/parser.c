
#include "ast.h"
#include "pixel.h"
#include "compiler.h"

Node* parse_expression(List* errors, List* tokens, size_t* i, char* filepath);

Node* parse_integer (List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    Node* int_node = (Node*)malloc(sizeof(Node));
    if (!int_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    int_node->kind = NODE_INTEGER;
    int_node->Node_Integer.value = token->value;
    int_node->line = token->line;
    int_node->pos = token->pos;

    (*i)++;
    return int_node;
};
Node* parse_decimal (List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    Node* dec_node = (Node*)malloc(sizeof(Node));
    if (!dec_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    dec_node->kind = NODE_DECIMAL;
    dec_node->Node_Decimal.value = token->value;
    dec_node->line = token->line;
    dec_node->pos = token->pos;

    (*i)++;
    return dec_node;
};

Node* parse_grouped_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    (*i)++; // Consume '('
    Node* expr = parse_expression(errors, tokens, i, filepath);
    if (!expr) return NULL;
    Token* token = tokens->items[*i];
    if (token->kind != TOKEN_RPAREN) {
        add_error_to_list(errors, "SyntaxError", "Unmatched Parentheses", "Expected a closing parenthesis", filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a closing parenthesis");
        return NULL;
    }
    (*i)++; // Consume ')'
    return expr;
}

Node* parse_primary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (token->kind == TOKEN_INTEGER) {
        return parse_integer(errors, tokens, i, filepath);
    } else if (token->kind == TOKEN_DECIMAL) {
        return parse_decimal(errors, tokens, i, filepath);
    } else if (token->kind == TOKEN_LPAREN) {
        return parse_grouped_expression(errors, tokens, i, filepath);
    } else {
        add_error_to_list(errors, "SyntaxError", "Invalid Expression", "Expected an expression", filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a primary expression in the primary expression function");
        return NULL;
    }
}

Node* parse_unary_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (
        token->kind == TOKEN_MINUS || token->kind == TOKEN_NOT
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
        unary_node->kind = NODE_UNARY;
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
        op_token->kind == TOKEN_PLUS || op_token->kind == TOKEN_MINUS ||
        op_token->kind == TOKEN_ASTERISK || op_token->kind == TOKEN_FWD_SLASH
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

        binary_node->kind = NODE_BINARY;
        binary_node->line = op_token->line;
        binary_node->pos = op_token->pos;
        binary_node->Node_Binary.left = left;
        binary_node->Node_Binary.right = right;
        binary_node->Node_Binary.operator = op_token->value;

        left = binary_node; // Update the left operand for the next iteration

        // Check for additional binary operators and create new binary expression nodes
        while (*i < tokens->length) {
            Token* nxt_tok = tokens->items[*i];
            if (
                nxt_tok->kind == TOKEN_PLUS || nxt_tok->kind == TOKEN_MINUS ||
                nxt_tok->kind == TOKEN_ASTERISK || nxt_tok->kind == TOKEN_FWD_SLASH
            ) {
                if (strcmp(op_token->value, nxt_tok->value) != 0) {
                    // If the operators are different, throw an error
                    add_error_to_list(errors, "SyntaxError", "Mixed Binary Operators", "Cannot mix different binary operators without parentheses", filepath, nxt_tok->line, nxt_tok->pos, __FILE__, __LINE__);
                    perror("Cannot mix different binary operators without parentheses");
                    return NULL;
                }
                
                (*i)++;
                Node* next_right = parse_unary_expression(errors, tokens, i, filepath);
                if (!next_right) {
                    perror("Failed to parse unary expression in the while loop of the binary expression");
                    return NULL;
                }

                // Create a new binary expression node and make the previous binary node its left child
                Node* new_binary_node = (Node*)malloc(sizeof(Node));
                if (!new_binary_node) {
                    perror("Failed to allocate memory for binary expression node");
                    exit(EXIT_FAILURE);
                }

                new_binary_node->kind = NODE_BINARY;
                new_binary_node->line = nxt_tok->line;
                new_binary_node->pos = nxt_tok->pos;
                new_binary_node->Node_Binary.left = left; // Use the previous binary node as the left operand
                new_binary_node->Node_Binary.right = next_right;
                new_binary_node->Node_Binary.operator = nxt_tok->value;

                left = new_binary_node; // Update the left operand for the next iteration
            } else {
                break;
            }
        }

        return left; // Return the last binary node created
    }

    return left;
}
Node* parse_expression(List* errors, List* tokens, size_t* i, char* filepath) {
    if (*i >= tokens->length) {
        size_t lastLine = ((Token*)tokens->items[tokens->length - 1])->line;
        size_t lastPos = ((Token*)tokens->items[tokens->length - 1])->pos;
        add_error_to_list(errors, "SyntaxError", "Incomplete Expression", "Expected an expression", filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression");
        return NULL;
    }
    Node* binary_expr = parse_binary_expression(errors, tokens, i, filepath);

    return binary_expr;
}


Node* parse_return(List* errors, List* tokens, size_t* i, char* filepath) {
    (*i)++;  // Consume 'return'

    if (*i >= tokens->length) {
        size_t lastLine = ((Token*)tokens->items[tokens->length - 1])->line;
        size_t lastPos = ((Token*)tokens->items[tokens->length - 1])->pos;
        
        add_error_to_list(errors, "SyntaxError", "Incomplete Return Statement", "Expected an expression after the 'return' statement", filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression after the 'return' statement");
        return NULL;
    }

    Node* expr_node = parse_expression(errors, tokens, i, filepath);
    if (!expr_node) {
        perror("Failed to parse expression in return statement");
        return NULL;
    }
    
    Node* ret_node = (Node*)malloc(sizeof(Node));
    if (!ret_node) {
        perror("Failed to allocate memory for return statement");
        exit(EXIT_FAILURE);
    }
    ret_node->kind = NODE_RETURN;
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

        if (token->kind == TOKEN_RETURN) {
            in_recovery_loop = false;
            Node* ret_node = parse_return(errors, tokens, i, filepath);
            if (!ret_node) {
                in_recovery_loop = true;
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
            (*i)++;
        }
    }
    return block;
}

bool parse_file(List* errors, Node* program, List* tokens, char* filepath) {
    bool ok = false;
    size_t i = 0;

    size_t current_error_count = errors->length;

    List* statements = parse_block(errors, tokens, &i, filepath);
    printf("\nNumber of Statements: %zu\n", statements->length);
    program->Node_Program.block = statements;

    return errors->length == current_error_count;
}