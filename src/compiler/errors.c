#include <stdio.h>

#include "pixel.h"
#include "compiler.h"

// Use perror for all memory allocation errors
// use CompilerError for all other errors

void compiler_error_print(CompilerError* error) {
    printf("Compiler Error: %s\n", error->category);
    printf("Header: %s\n", error->header);
    printf("Message: %s\n", error->msg);

    printf("Pixel File: %s\n", error->pixel_file);
    printf("Pixel Line: %zu\n", error->pixel_line);
    printf("Pixel Position: %zu\n", error->pixel_pos);
    
    printf("C File: %s\n", error->c_file);
    printf("C Line: %zu\n", error->c_line);
}

void add_error_to_list(List* CompilerErrors, char* category, char* header, char* msg, char* pixel_file, size_t pixel_line, size_t pixel_pos, char* c_file, size_t c_line) {
    CompilerError* error = (CompilerError*)malloc(sizeof(CompilerError));
    if (!error) {
        perror("Memory allocation failed for CompilerError");
        exit(EXIT_FAILURE);
    }
    error->category = category;
    error->header = header;
    error->msg = msg;
    error->pixel_file = pixel_file;
    error->pixel_line = pixel_line;
    error->pixel_pos = pixel_pos;
    error->c_file = c_file;
    error->c_line = c_line;
    list_push(CompilerErrors, error);
}