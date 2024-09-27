#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

CompilerError* parse_integer(Leaf* int_node, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "INT") != 0) {
        CompilerError* error = (CompilerError*)malloc(sizeof(CompilerError));
        if (!error) {
            perror("Failed to allocate memory for CompilerError");
            exit(EXIT_FAILURE);
        }
        size_t msg_len = strlen("Expected an integer, but instead found ") + strlen(token->value) + 1;
        char* msg = (char*)malloc(msg_len);
        if (!msg) {
            perror("Failed to allocate memory for error message");
            exit(EXIT_FAILURE);
        }
        snprintf(msg, msg_len, "Expected an integer, but instead found %s", token->value);
        error->category = "SyntaxError";
        error->header = "Invalid Integer";
        error->msg = msg;
        error->pixel_file = filepath;
        error->pixel_line = token->line;
        error->pixel_pos = token->pos;
        error->c_file = __FILE__;
        error->c_line = __LINE__;
        return error;
    }
    int_node->kind = "INT";
    int_node->value = token->value;
    int_node->pos = token->pos;
    int_node->line = token->line;
    int_node->args = NULL;
    int_node->statements = NULL;
    return NULL;
}
CompilerError* parse_unary_expression(Leaf* expr_node, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "MINUS") == 0) {
        (*i)++;
        Leaf* operand = (Leaf*)malloc(sizeof(Leaf));
        if (!operand) {
            perror("Failed to allocate memory for operand");
            exit(EXIT_FAILURE);
        }
        CompilerError* error = parse_expression(operand, tokens, i, filepath);
        if (error) {
            return error;
        }
        expr_node->kind = "UNARY_MINUS";
        expr_node->value = NULL;
        expr_node->pos = token->pos;
        expr_node->line = token->line;
        expr_node->args = list_init("List<Leaf>");
        if (!expr_node->args) {
            perror("Failed to allocate memory for unary expression arguments");
            exit(EXIT_FAILURE);
        }
        list_push(expr_node->args, operand);
        return NULL;
    }
    return parse_integer(expr_node, tokens, i, filepath);
}
CompilerError* parse_binary_expression(Leaf* expr_node, List* tokens, size_t* i, char* filepath) {
    CompilerError* error = parse_unary_expression(expr_node, tokens, i, filepath);
    if (error) {
        return error;
    }
    
    while (*i < tokens->length) {
        Token* token = tokens->items[*i];
        if (strcmp(token->kind, "PLUS") == 0 || strcmp(token->kind, "MINUS") == 0) {
            (*i)++;
            Leaf* right_operand = (Leaf*)malloc(sizeof(Leaf));
            if (!right_operand) {
                perror("Failed to allocate memory for right operand");
                exit(EXIT_FAILURE);
            }
            
            error = parse_unary_expression(right_operand, tokens, i, filepath);
            if (error) {
                return error;
            }
            
            Leaf* new_expr = (Leaf*)malloc(sizeof(Leaf));
            if (!new_expr) {
                perror("Failed to allocate memory for new expression");
                exit(EXIT_FAILURE);
            }
            
            new_expr->kind = (strcmp(token->kind, "PLUS") == 0) ? "BINARY_PLUS" : "BINARY_MINUS";
            new_expr->value = NULL;
            new_expr->pos = token->pos;
            new_expr->line = token->line;
            new_expr->args = list_init("List<Leaf>");
            if (!new_expr->args) {
                perror("Failed to allocate memory for binary expression arguments");
                exit(EXIT_FAILURE);
            }
            
            list_push(new_expr->args, expr_node);
            list_push(new_expr->args, right_operand);
            
            expr_node = new_expr;  // Update expr_node to the new binary expression
        } else {
            break;
        }
    }
    
    return NULL;
}

CompilerError* parse_expression(Leaf* expr_node, List* tokens, size_t* i, char* filepath) {
    
    return parse_binary_expression(expr_node, tokens, i, filepath);
}