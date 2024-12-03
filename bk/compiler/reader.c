#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pixel.h"
#include "compiler.h"

bool read_file(List* CompilerErrors, char** src, char* filename) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        add_error_to_list(CompilerErrors, "FileIOError", "Failed to open file", "Failed to open file", filename, 0, 0, NULL, 10);
        return false;
    }

    // Calculate the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer to hold the file contents
    *src = (char*)malloc(file_size + 1);
    if (*src == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Read the file contents into the src buffer
    size_t bytes_read = fread(*src, 1, file_size, file);
    (*src)[bytes_read] = '\0'; // Null-terminate the src string

    // Close the file
    fclose(file);

    return true;
}