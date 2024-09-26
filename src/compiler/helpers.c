#include <stdio.h>
#include <stdlib.h>

void fatal(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

char* join_dyn_strings(char* s1, char* s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*)malloc(len1 + len2 + 1);
    if (!result) {
        perror("Failed to allocate memory for dynamic string");
        exit(EXIT_FAILURE);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}