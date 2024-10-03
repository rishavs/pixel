#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void fatal(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
char* join_strings(const char* first, ...) {
    va_list args;
    size_t total_length = 0;
    const char* current;

    // First pass: calculate total length
    va_start(args, first);
    current = first;
    while (current != NULL) {
        if (current != NULL) {
            total_length += strlen(current);
        }
        current = va_arg(args, const char*);
    }
    va_end(args);

    // Allocate memory for the result
    char* result = (char*)malloc(total_length + 1);
    if (!result) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Second pass: copy strings
    char* dest = result;
    va_start(args, first);
    current = first;
    while (current != NULL) {
        if (current != NULL) {
            size_t len = strlen(current);
            memcpy(dest, current, len);
            dest += len;
        }
        current = va_arg(args, const char*);
    }
    va_end(args);

    *dest = '\0';  // Null-terminate the result

    return result;
}