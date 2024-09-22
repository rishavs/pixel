#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pixel.h"

bool read_file(List* errors, char** src, char* filename) {
    Error* dummy_error = (Error*)malloc(sizeof(Error));
    if (!dummy_error) {
        perror("Memory allocation failed for dummy_error");
        return false;
    }
    *dummy_error = (Error){
        .type = "FileError",
        .file = __FILE__,
        .header = "Failed to open file",
        .msg = "Failed to open file",
        .line = __LINE__,
        .pos = 0,
        .fatal = true
    };
    list_push(errors, dummy_error);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        Error error = {
            .type = "FileError",
            .file = filename,
            .header = "Failed to open file",
            .msg = "Failed to open file",
            .line = 0,
            .pos = 0,
            .fatal = true
        };
        list_push(errors, &error);
        return false;
    }

    // Calculate the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the buffer to hold the file contents
    *src = (char*)malloc(file_size + 1);
    if (*src == NULL) {
        Error error = {
            .type = "MemoryError",
            .file = filename,
            .header = "Memory allocation failed",
            .msg = "Memory allocation failed",
            .line = 0,
            .pos = 0,
            .fatal = true
        }; 
        list_push(errors, &error);
        fclose(file);
        return false;
    }

    // Read the file contents into the src buffer
    size_t bytes_read = fread(*src, 1, file_size, file);
    (*src)[bytes_read] = '\0'; // Null-terminate the src string

    // Close the file
    fclose(file);

    return true;
}