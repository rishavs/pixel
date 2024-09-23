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

void add_error_to_list(List* CompilerErrors, const char* category, const char* header, const char* msg, const char* pixel_file, size_t pixel_line, size_t pixel_pos, const char* c_file, size_t c_line) {
    CompilerError* error = (CompilerError*)malloc(sizeof(CompilerError));
    if (!error) {
        perror("Memory allocation failed for compiler_error");
        return;
    }
    strncpy(error->category, category, COMPILER_ERROR_MAX_CATEGORY_LENGTH - 1);
    error->category[COMPILER_ERROR_MAX_CATEGORY_LENGTH - 1] = '\0';

    strncpy(error->header, header, COMPILER_ERROR_MAX_HEADER_LENGTH - 1);
    error->header[COMPILER_ERROR_MAX_HEADER_LENGTH - 1] = '\0';

    strncpy(error->msg, msg, COMPILER_ERROR_MAX_MSG_LENGTH - 1);
    error->msg[COMPILER_ERROR_MAX_MSG_LENGTH - 1] = '\0';

    strncpy(error->pixel_file, pixel_file, COMPILER_ERROR_MAX_FILE_LENGTH - 1);
    error->pixel_file[COMPILER_ERROR_MAX_FILE_LENGTH - 1] = '\0';

    error->pixel_line = pixel_line;
    error->pixel_pos = pixel_pos;

    strncpy(error->c_file, (c_file != NULL) ? c_file : "", COMPILER_ERROR_MAX_FILE_LENGTH - 1);
    error->c_file[COMPILER_ERROR_MAX_FILE_LENGTH - 1] = '\0';

    error->c_line = c_line;

    list_push(CompilerErrors, error);
}