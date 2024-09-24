#include "compiler.h"

bool parse_file (List* errors, Leaf* program, List* tokens, char* filepath){
    Leaf* block = (Leaf*)malloc(sizeof(Leaf));
    if (!block) {
        add_error_to_list(errors, "Memory", "Allocation", "Failed to allocate memory for block", filepath, 0, 0, __FILE__, __LINE__);
        return false;
    }
    block->kind = "BLOCK";
    block->value = NULL;
    block->pos = 0;
    block->line = 0;
    block->args = NULL;
    block->statements = list_init("List<Leaf>");
    if (!block->statements) {
        add_error_to_list(errors, "Memory", "Allocation", "Failed to allocate memory for block statements", filepath, 0, 0, __FILE__, __LINE__);
        return false;
    }
    program->statements = block;
    return true;
    
}

bool parse_block (List* errors, Leaf* block, List* tokens, char* filepath){
    size_t i = 0;
    while (tokens->length > 0) {
        Token* token = tokens->items[i];

        if (strcmp(token->kind, "RETURN") == 0) {
            Leaf* ret = (Leaf*)malloc(sizeof(Leaf));
            if (!ret) {
                add_error_to_list(errors, "Memory", "Allocation", "Failed to allocate memory for function", filepath, token->line, token->pos, __FILE__, __LINE__);
                return false;
            }
            ret->kind = "RETURN";
            ret->value = token->value;
            ret->pos = token->pos;
            ret->line = token->line;
            ret->args = NULL;
            ret->statements = list_init("List<Leaf>");
            if (!ret->statements) {
                add_error_to_list(errors, "Memory", "Allocation", "Failed to allocate memory for function statements", filepath, token->line, token->pos, __FILE__, __LINE__);
                return false;
            }
            list_push(block->statements, ret);
            if (!parse_ret(errors, ret, tokens, filepath)) {
                return false;
            }
        } else {
            char* var = token->value;
            add_error_to_list(errors, "Syntax", "Error", "Unexpected token", filepath, token->line, token->pos, __FILE__, __LINE__);
            return false;
        }

        i++;
    }
    
}

bool parse_statement (List* errors, Leaf* var, List* tokens, char* filepath){
    
}

bool parse_ret (List* errors, Leaf* func, List* tokens, char* filepath){
    
}