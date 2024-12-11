#ifndef PIXEL_HELPERS_H
#define PIXEL_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

// char* to_uri_safe_string(const char* str);

bool string_starts_with(char *src, size_t src_len, size_t i,  char* frag);
char* get_substring(char* src, size_t pos, size_t len);
char* join_strings(int n, ...);
char* build_string (const char* format, ...);

char* get_duration(clock_t start, clock_t end);

#endif // PIXEL_HELPERS_H