#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void fatal(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

char* join_strings(int n, ...) {
    va_list args;
    size_t total_length = 0;

    // First pass: calculate total length
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        const char* str = va_arg(args, const char*);
        total_length += strlen(str);
    }
    va_end(args);

    // Allocate memory for the result
    char* result = (char*)malloc(total_length + 1); // +1 for the null terminator
    if (!result) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Second pass: copy strings
    result[0] = '\0'; // Initialize the result as an empty string
    va_start(args, n);
    for (int i = 0; i < n; i++) {
        const char* str = va_arg(args, const char*);
        strcat(result, str);
    }
    va_end(args);

    return result;
}

// Function to convert a string to a URI-safe string
char* to_uri_safe_string(const char* str) {
    size_t length = strlen(str);
    char* uri_safe_str = (char*)malloc(length * 3 + 1); // Allocate maximum possible size
    if (!uri_safe_str) {
        perror("Failed to allocate memory for URI-safe string");
        return NULL;
    }

    char* p = uri_safe_str;
    for (size_t i = 0; i < length; i++) {
        if (str[i] == '/' || str[i] == '\\') {
            sprintf(p, "%%%02X", (unsigned char)str[i]);
            p += 3;
        } else {
            *p++ = str[i];
        }
    }
    *p = '\0';
    return uri_safe_str;
}
