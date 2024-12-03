#include "ast.h"
#include "pixel.h"
#include "compiler.h"

typedef struct {
    int current_depth;
    Node* current_scope_owner; // who owns the statement block/scope

    List* errors;
    List* tokens;
    
    char* filepath;
    size_t i;

} Parsing_context;

Node* parse_expression(Parsing_context* ctx);

Node* parse_integer (Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    Node* int_node = (Node*)malloc(sizeof(Node));
    if (!int_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    int_node->kind = NODE_INTEGER;
    int_node->Node_Integer.value = token->value;
    int_node->line = token->line;
    int_node->pos = token->pos;
    int_node->depth = ctx->current_depth;
    int_node->scope_owner = ctx->current_scope_owner;

    ctx->i ++;
    return int_node;
};

Node* parse_decimal (Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    Node* dec_node = (Node*)malloc(sizeof(Node));
    if (!dec_node) {
        perror("Failed to allocate memory for integer node");
        exit(EXIT_FAILURE);
    }
    dec_node->kind = NODE_DECIMAL;
    dec_node->Node_Decimal.value = token->value;
    dec_node->line = token->line;
    dec_node->pos = token->pos;
    dec_node->depth = ctx->current_depth;
    dec_node->scope_owner = ctx->current_scope_owner;

    ctx->i ++;
    return dec_node;
};

Node* parse_identifier (Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    Node* ident_node = (Node*)malloc(sizeof(Node));
    if (!ident_node) {
        perror("Failed to allocate memory for identifier node");
        exit(EXIT_FAILURE);
    }
    ident_node->kind = NODE_IDENTIFIER;
    ident_node->Node_Identifier.value = token->value;
    ident_node->line = token->line;
    ident_node->pos = token->pos;
    ident_node->depth = ctx->current_depth;
    ident_node->scope_owner = ctx->current_scope_owner;

    ctx->i ++;
    return ident_node;
};

Node* parse_grouped_expression(Parsing_context* ctx) {
    ctx->i ++; // Consume '('
    Node* expr = parse_expression(ctx);
    if (!expr) return NULL;
    Token* token = ctx->tokens->items[ctx->i];
    if (token->kind != TOKEN_RPAREN) {
        add_error_to_list(ctx->errors, "SyntaxError", "Unmatched Parentheses", "Expected a closing parenthesis", ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a closing parenthesis");
        return NULL;
    }
    ctx->i ++; // Consume ')'
    return expr;
}

Node* parse_primary_expression(Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    if (token->kind == TOKEN_INTEGER) {
        return parse_integer(ctx);
    } else if (token->kind == TOKEN_DECIMAL) {
        return parse_decimal(ctx);
    } else if (token->kind == TOKEN_LPAREN) {
        return parse_grouped_expression(ctx);
    } else if (token->kind == TOKEN_IDENTIFIER) {
        return parse_identifier(ctx);
    } else {
        add_error_to_list(ctx->errors, "SyntaxError", "Invalid Expression", "Expected an expression", ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
        perror("Expected a primary expression in the primary expression function");
        return NULL;
    }
}

Node* parse_unary_expression(Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];
    if (
        token->kind == TOKEN_MINUS || token->kind == TOKEN_NOT
    ) {
        ctx->i ++;
        Node* unr_expr = parse_primary_expression(ctx);
        if (!unr_expr) {
            perror("Failed to parse primary expression in the unary function");
            return NULL;
        }
        Node* unary_node = (Node*)malloc(sizeof(Node));
        if (!unary_node) {
            perror("Failed to allocate memory for unary expression node");
            exit(EXIT_FAILURE);
        }
        unr_expr->parent = unary_node;

        unary_node->kind = NODE_UNARY;
        unary_node->line = token->line;
        unary_node->pos = token->pos;
        unary_node->Node_Unary.right = unr_expr;
        unary_node->Node_Unary.operator = token->value;
        unary_node->depth = ctx->current_depth;
        unary_node->scope_owner = ctx->current_scope_owner;

        return unary_node;
    } else {
        Node* expr = parse_primary_expression(ctx);
        if (!expr) {
            perror("Failed to parse primary expression in the else branch of unary function");
            return NULL;
        }
        return expr;
    }
}

Node* parse_binary_expression(Parsing_context* ctx) {
    Node* left = parse_unary_expression(ctx);
    if (!left) {
        perror("Failed to parse the left unary expression in binary expression");
        return NULL;
    }
    if (ctx->i >= ctx->tokens->length) return left;

    Token* op_token = ctx->tokens->items[ctx->i];
    if (
        op_token->kind == TOKEN_PLUS || op_token->kind == TOKEN_MINUS ||
        op_token->kind == TOKEN_ASTERISK || op_token->kind == TOKEN_FWD_SLASH
    ) {
        ctx->i ++;
        Node* right = parse_unary_expression(ctx);
        if (!right) {
            perror("Failed to parse unary expression in binary expression");
            return NULL;
        }

        Node* binary_node = (Node*)malloc(sizeof(Node));
        if (!binary_node) {
            perror("Failed to allocate memory for binary expression node");
            exit(EXIT_FAILURE);
        }
        left->parent = binary_node;
        right->parent = binary_node;

        binary_node->kind = NODE_BINARY;
        binary_node->line = op_token->line;
        binary_node->pos = op_token->pos;
        binary_node->Node_Binary.operator = op_token->value;
        binary_node->Node_Binary.expressions = list_init("List<Node>");
        binary_node->depth = ctx->current_depth;
        binary_node->scope_owner = ctx->current_scope_owner;

        list_push(binary_node->Node_Binary.expressions, left); // Add left expression to the list
        list_push(binary_node->Node_Binary.expressions, right); // Add right expression to the list

        // Check for additional binary operators and add new expressions to the list
        while (ctx->i < ctx->tokens->length) {
            Token* nxt_tok = ctx->tokens->items[ctx->i];
            if (
                nxt_tok->kind == TOKEN_PLUS || nxt_tok->kind == TOKEN_MINUS ||
                nxt_tok->kind == TOKEN_ASTERISK || nxt_tok->kind == TOKEN_FWD_SLASH
            ) {
                if (strcmp(op_token->value, nxt_tok->value) != 0) {
                    // If the operators are different, throw an error
                    add_error_to_list(ctx->errors, "SyntaxError", "Mixed Binary Operators", "Cannot mix different binary operators without parentheses", ctx->filepath, nxt_tok->line, nxt_tok->pos, __FILE__, __LINE__);
                    perror("Cannot mix different binary operators without parentheses");
                    return NULL;
                }
                
                ctx->i ++;
                Node* next_expr = parse_unary_expression(ctx);
                if (!next_expr) {
                    perror("Failed to parse unary expression in the while loop of the binary expression");
                    return NULL;
                }

                list_push(binary_node->Node_Binary.expressions, next_expr); // Add next expression to the list
            } else {
                break;
            }
        }

        return binary_node; // Return the binary node with the list of expressions
    }

    return left;
}

Node* parse_expression(Parsing_context* ctx) {
    if (ctx->i >= ctx->tokens->length) {
        size_t lastLine = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->line;
        size_t lastPos = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->pos;
        add_error_to_list(ctx->errors, "SyntaxError", "Incomplete Expression", "Expected an expression", ctx->filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression");
        return NULL;
    }
    Node* binary_expr = parse_binary_expression(ctx);

    return binary_expr;
}

Node* parse_declaration(Parsing_context* ctx) {
    Token* token = ctx->tokens->items[ctx->i];

    Node* decl_node = (Node*)malloc(sizeof(Node));
    if (!decl_node) {
        perror("Failed to allocate memory for declaration node");
        exit(EXIT_FAILURE);
    }
    decl_node->kind = NODE_DECLARATION;
    decl_node->line = token->line;
    decl_node->pos = token->pos;
    decl_node->depth = ctx->current_depth;
    decl_node->scope_owner = ctx->current_scope_owner;

    if (token->kind == TOKEN_LET) {
        decl_node->Node_Declaration.is_new = true;
        ctx->i++; // Consume 'let'
        token = ctx->tokens->items[ctx->i];
    }

    if (token->kind == TOKEN_VAR) {
        decl_node->Node_Declaration.is_var = true;
        ctx->i++; // Consume 'var'
        token = ctx->tokens->items[ctx->i];
    }

    if (token->kind != TOKEN_IDENTIFIER) {
        add_error_to_list(ctx->errors, "SyntaxError", "Invalid Declaration", "Expected an identifier in the declaration", ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
    }

    Node* ident_node = parse_identifier(ctx);
    if (!ident_node) {
        perror("Failed to parse identifier in declaration");
        return NULL;
    }
    decl_node->Node_Declaration.identifier = ident_node;
    token = ctx->tokens->items[ctx->i];

    // if there is an assignment operator, parse the expression
    // else return the declaration node

    if (token->kind != TOKEN_ASSIGN) {
        decl_node->Node_Declaration.is_assignment = false;
        return decl_node;
    }

    decl_node->Node_Declaration.is_assignment = true;
    ctx->i++; // Consume '='
    Node* expr_node = parse_expression(ctx);
    if (!expr_node) {
        perror("Failed to parse expression in declaration");
        return NULL;
    }
    decl_node->Node_Declaration.expr = expr_node;
    return decl_node;
}

Node* parse_return(Parsing_context* ctx) {
    ctx->i++;  // Consume 'return'

    if (ctx->i >= ctx->tokens->length) {
        size_t lastLine = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->line;
        size_t lastPos = ((Token*)ctx->tokens->items[ctx->tokens->length - 1])->pos;
        
        add_error_to_list(ctx->errors, "SyntaxError", "Incomplete Return Statement", "Expected an expression after the 'return' statement", ctx->filepath, lastLine, lastPos, __FILE__, __LINE__);
        perror("Expected an expression after the 'return' statement");
        return NULL;
    }

    Node* expr_node = parse_expression(ctx);
    if (!expr_node) {
        perror("Failed to parse expression in return statement");
        return NULL;
    }
    
    Node* ret_node = (Node*)malloc(sizeof(Node));
    if (!ret_node) {
        perror("Failed to allocate memory for return statement");
        exit(EXIT_FAILURE);
    }
    expr_node->parent = ret_node;

    ret_node->kind = NODE_RETURN;
    ret_node->line = expr_node->line;
    ret_node->pos = expr_node->pos;
    ret_node->Node_Return.expr = expr_node;

    ret_node->depth = ctx->current_depth;
    ret_node->scope_owner = ctx->current_scope_owner;

    return ret_node;
}

List* parse_block(Parsing_context* ctx) {
    List* block = list_init("List<Node>");

    bool in_recovery_loop = false;

    // increment the depth
    ctx->current_depth++;

    while (ctx->i < ctx->tokens->length) {
        Token* token = ctx->tokens->items[ctx->i];
        Token* next_token = ctx->i + 1 < ctx->tokens->length ? ctx->tokens->items[ctx->i + 1] : NULL;
        in_recovery_loop = false;

        if (token->kind == TOKEN_LET || token->kind == TOKEN_VAR) {
            Node* decl_node = parse_declaration(ctx);
            if (!decl_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, decl_node);

        } else if (token->kind == TOKEN_IDENTIFIER && next_token && next_token->kind == TOKEN_ASSIGN) {
            Node* decl_node = parse_declaration(ctx);
            if (!decl_node) {
                in_recovery_loop = true;
                return block;
            }
            list_push(block, decl_node);

        } else if (token->kind == TOKEN_RETURN) {
            Node* ret_node = parse_return(ctx);
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
                add_error_to_list(ctx->errors, "SyntaxError", "Unexpected Token", err_msg,  ctx->filepath, token->line, token->pos, __FILE__, __LINE__);
            }
            ctx->i++;
        }
    }
    // decrement the depth
    ctx->current_depth--;

    return block;
}

bool parse_file(List* errors, Node* program, List* tokens, char* filepath) {
    size_t current_error_count = errors->length;

    Parsing_context* ctx = (Parsing_context*)malloc(sizeof(Parsing_context));
    if (!ctx) {
        perror("Failed to allocate memory for parser context");
        exit(EXIT_FAILURE);
    }
    ctx->current_depth          = 0;
    ctx->errors                 = errors;
    ctx->tokens                 = tokens;
    ctx->filepath               = filepath;
    ctx->i                      = 0;
    ctx->current_scope_owner    = program;

    List* statements = parse_block(ctx);
    for (size_t j = 0; j < statements->length; j++) {
        Node* stmt = statements->items[j];
        stmt->parent = program;
        stmt->depth = 1;
    }

    printf("\nNumber of Statements: %zu\n", statements->length);
    program->Node_Program.block = statements;

    return errors->length == current_error_count;
}