#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fatal_memory_allocation_failure(const char* file, int line) {
    fprintf(stderr, "\033[0;31m[ FATAL ] \033[0m"); // Red color for FAILURE
    perror("Memory Allocation Failed!");
    fprintf(stderr, "Note: This is an internal compiler bug at %s:%d. Please raise a bug with the minimal reproducible code.\n", file, line);
    exit(EXIT_FAILURE);
}