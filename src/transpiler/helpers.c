#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

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

char* get_duration(clock_t start, clock_t end) {
    double duration = ((double)(end - start))/CLOCKS_PER_SEC; // in seconds

    int minutes = (int)duration / 60;
    int seconds = (int)duration % 60;
    int milliseconds = ((int)(duration * 1000)) % 1000;
    int microseconds = ((int)(duration * 1000000)) % 1000;

    char *formatted_duration = malloc(100 * sizeof(char));
    if (formatted_duration == NULL) {
        perror("Failed to allocate memory for formatted_duration");
        exit(EXIT_FAILURE);
    }

    snprintf(formatted_duration, 100, "%dm %ds %dms %dus", 
             minutes, seconds, milliseconds, microseconds);

    return formatted_duration;
}

// wrap sprintf to return the string
char* build_string (const char* format, ...) {
    va_list args;
    va_start(args, format);

    // get the length of the formatted string
    size_t len = vsnprintf(NULL, 0, format, args) + 1;
    va_end(args);

    // allocate memory for the formatted string
    char* buffer = calloc(len, sizeof(char));
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }

    // format the string
    va_start(args, format);
    vsnprintf(buffer, len, format, args);
    va_end(args);

    return buffer;
}