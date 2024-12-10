#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool string_starts_with(char *src, size_t src_len, size_t i,  char* frag) {
    size_t frag_len = strlen(frag);
    if (i + frag_len > src_len) { return false; }
    for (size_t j = 0; j < frag_len; j++) {
        if (src[i + j] != frag[j]) { return false; }
    }
    return true;
}

char* get_substring(char* src, size_t pos, size_t len) {
    if (src == NULL) return NULL;

    char* buffer = calloc(len + 1, sizeof(char));
    if (buffer == NULL) return NULL;
    
    strncpy(buffer, src + pos, len);
    buffer[len] = '\0';
    return buffer;
}

char* join_strings(int n, ...) {
    if (n == 0) return NULL;

    va_list args;
    va_start(args, n);

    size_t total_len = 0;
    for (int i = 0; i < n; i++) {
        char* str = va_arg(args, char*);
        if (str != NULL) {
            total_len += strlen(str);
        }
    }
    va_end(args);

    char* buffer = calloc(total_len + 1, sizeof(char));
    if (buffer == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    va_start(args, n);
    for (int i = 0; i < n; i++) {
        char* str = va_arg(args, char*);
        if (str != NULL) {
            strcat(buffer, str);
        }
    }
    va_end(args);

    return buffer;
}
// char* result = join_strings(3, "Hello, ", "World", "!");
// if (result != NULL) {
//     printf("%s\n", result);
//     free(result);
// }