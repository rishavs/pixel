#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


CompilerError* parse_integer(Leaf* int_node, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "INT") != 0) {
        CompilerError* error = (CompilerError*)malloc(sizeof(CompilerError));
        if (!error) {
            perror("Failed to allocate memory for CompilerError");
            exit(EXIT_FAILURE);
        }
        // char* msg = "Expected an integer" + token->value;
        char* msg = strcat("Expected an integer, but instead found ", token->value);
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

    *i ++;

    return NULL;
}


CompilerError* parse_expression(Leaf* expr_node, List* tokens, size_t* i, char* filepath) {
    Token* token = tokens->items[*i];
    if (strcmp(token->kind, "INT") == 0) {
        return parse_integer(expr_node, tokens, i, filepath);
    } 

    CompilerError* error = (CompilerError*)malloc(sizeof(CompilerError));
    if (!error) {
        perror("Failed to allocate memory for CompilerError");
        exit(EXIT_FAILURE);
    }
    error->category = "SyntaxError";
    error->header = "Invalid Expression";
    error->msg = "Expected an integer expression";
    error->pixel_file = filepath;
    error->pixel_line = token->line;
    error->pixel_pos = token->pos;
    error->c_file = __FILE__;
    error->c_line = __LINE__;
    return error;
}

CompilerError* parse_return(Leaf* ret_node, List* tokens, size_t* i, char* filepath) {
    (*i)++;
    if (*i >= tokens->length) {
        CompilerError* error = (CompilerError*)malloc(sizeof(CompilerError));
        if (!error) {
            perror("Failed to allocate memory for CompilerError");
            exit(EXIT_FAILURE);
        }
        error->category = "SyntaxError";
        error->header = "Incomplete Return Statement";
        error->msg = "Expected an expression after 'return'";
        error->pixel_file = filepath;
        error->pixel_line = ret_node->line;
        error->pixel_pos = ret_node->pos;
        error->c_file = __FILE__;
        error->c_line = __LINE__;
        return error;
    }

    Leaf* expr_node = (Leaf*)malloc(sizeof(Leaf));
    if (!expr_node) {
        perror("Failed to allocate memory for expression node");
        exit(EXIT_FAILURE);
    }

    CompilerError* error = parse_expression(expr_node, tokens, i, filepath);
    if (error)  return error;

    list_push(ret_node->args, expr_node);
    
    return NULL;
}

bool parse_block(List* errors, Leaf* block, List* tokens, size_t* i, char* filepath) {
    bool in_recovery_loop = false;
    bool ok = true;

    while (*i < tokens->length) {
        Token* token = tokens->items[*i];

        if (strcmp(token->kind, "RETURN") == 0) {
            in_recovery_loop = false;

            Leaf* ret_node = (Leaf*)malloc(sizeof(Leaf));
            if (!ret_node) {
                perror("Failed to allocate memory for return statement");
                exit(EXIT_FAILURE);
            }

            ret_node->kind = "RETURN";
            ret_node->value = NULL;
            ret_node->pos = token->pos;
            ret_node->line = token->line;
            ret_node->args = list_init("List<Leaf>");
            ret_node->statements = NULL;

            CompilerError* error = parse_return(ret_node, tokens, i, filepath);
            if (error) {
                add_error_to_list(errors, error->category, error->header, error->msg, error->pixel_file, error->pixel_line, error->pixel_pos, error->c_file, error->c_line);
                free(error);
                ok = false;
            } else {
                list_push(block->statements, ret_node);
            }

        } else {
            if (!in_recovery_loop) {
                in_recovery_loop = true;
                ok = false;
             
                char* msg = "Expected a statement, but instead found: ";
                size_t msg_len = strlen(msg) + strlen(token->value) + 1;
                char* err_msg = (char*)malloc(msg_len);
                if (!err_msg) {
                    perror("Failed to allocate memory for error message");
                    return false;
                }
                snprintf(err_msg, msg_len, "%s%s", msg, token->value);
                add_error_to_list(errors, "SyntaxError", "Unexpected Token", err_msg,  filepath, token->line, token->pos, __FILE__, __LINE__);
            }
        }
        (*i)++;
    }
    return ok;
}

bool parse_file(List* errors, Leaf* program, List* tokens, char* filepath) {
    size_t i = 0;

    Leaf* block = (Leaf*)malloc(sizeof(Leaf));
    if (!block) {
        perror("Failed to allocate memory for block statement");
        return false;
    }

    block->kind = "BLOCK";
    block->value = NULL;
    block->pos = 0;
    block->line = 0;
    block->args = NULL;
    block->statements = list_init("List<Leaf>");
    if (!block->statements) {
        perror("Failed to allocate memory for block statements");
        free(block);
        return false;
    }
    bool ok = parse_block(errors, block, tokens, &i, filepath);
    if (ok) {
        list_push(program->statements, block);
    } 
    return ok;
}