#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "resources.h"
#include "transpiler.h"

char* get_line(const char* src, size_t pos);
char* get_previous_line(const char* src, size_t pos);
char* get_next_line(const char* src, size_t pos);

void print_errors(Transpiler_context_t* ctx) {
    if (ctx->errors_count == 0) return;
    fprintf(stderr, "\033[0;31m-------------------------------------------\n");
    fprintf(stderr, "!! Compilation Failed !! Found %zu error(s)\n", ctx->errors_count);
    fprintf(stderr, "-------------------------------------------\n");

    for (size_t i = 0; i < ctx->errors_count; i++) {
        Transpiler_error_t e = ctx->errors[i];

        // Fetch and print the lines around the error
        char* prev_line = get_previous_line(ctx->src, e.pos);
        char* error_line = get_line(ctx->src, e.pos);
        char* next_line = get_next_line(ctx->src, e.pos);

        if (prev_line) {
            fprintf(stderr, "Previous line: %s\n", prev_line);
            free(prev_line);
        }
        if (e.line > 1) fprintf(stderr, "\t%zu | ... \n", e.line - 1); // Underline the error line
        fprintf(stderr, "\t%zu | \033[4m%s\033[0m\n", e.line, error_line); // Underline the error line
        fprintf(stderr, "\t%zu | ... \n", e.line + 1); 

        // Print the squiggly line
        size_t start = e.pos;
        while (start > 0 && ctx->src[start - 1] != '\n') {
            start--;
        }
        size_t offset = e.pos - start;
        fprintf(stderr, "%*s^\n", (int)offset, ""); // Print spaces followed by a caret

        fprintf(stderr, "%zu:\t%s %s\n\t", i, e.category, e.msg);
        fprintf(stderr, en_us[RES_PROBLEM_AT], e.pos, e.line, e.filepath);
        fprintf(stderr, "\n");


        if (next_line) {
            fprintf(stderr, "Next line: %s\n", next_line);
            free(next_line);
        }
        free(error_line);

        fprintf(stderr, "\t");
        perror(en_us[RES_INTERNAL_PERROR]);
        
        fprintf(stderr, "\t");
        fprintf(stderr, en_us[RES_INTERNAL_LOCATION], e.transpiler_line, e.transpiler_file);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\033[0m");
}

char* get_line(const char* src, size_t pos) {
    size_t start = pos;
    size_t end = pos;

    // Find the start of the line
    while (start > 0 && src[start - 1] != '\n') {
        start--;
    }

    // Find the end of the line
    while (src[end] != '\0' && src[end] != '\n') {
        end++;
    }

    // Allocate memory for the line
    size_t len = end - start;
    char* line = (char*)malloc(len + 1);
    if (line == NULL) {
        perror("Failed to allocate memory for line");
        exit(EXIT_FAILURE);
    }

    // Copy the line
    strncpy(line, src + start, len);
    line[len] = '\0';

    return line;
}

char* get_previous_line(const char* src, size_t pos) {
    if (pos == 0) return NULL;

    size_t end = pos;
    size_t start = pos;

    // Find the end of the previous line
    while (end > 0 && src[end - 1] != '\n') {
        end--;
    }

    // Find the start of the previous line
    if (end == 0) return NULL;
    start = end - 1;
    while (start > 0 && src[start - 1] != '\n') {
        start--;
    }

    // Allocate memory for the line
    size_t len = end - start;
    char* line = (char*)malloc(len + 1);
    if (line == NULL) {
        perror("Failed to allocate memory for previous line");
        exit(EXIT_FAILURE);
    }

    // Copy the line
    strncpy(line, src + start, len);
    line[len] = '\0';

    return line;
}

char* get_next_line(const char* src, size_t pos) {
    size_t start = pos;
    size_t end = pos;

    // Find the start of the next line
    while (src[start] != '\0' && src[start] != '\n') {
        start++;
    }
    if (src[start] == '\0') return NULL;
    start++;

    // Find the end of the next line
    end = start;
    while (src[end] != '\0' && src[end] != '\n') {
        end++;
    }

    // Allocate memory for the line
    size_t len = end - start;
    char* line = (char*)malloc(len + 1);
    if (line == NULL) {
        perror("Failed to allocate memory for next line");
        exit(EXIT_FAILURE);
    }

    // Copy the line
    strncpy(line, src + start, len);
    line[len] = '\0';

    return line;
}