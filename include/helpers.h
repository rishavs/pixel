#ifndef PIXEL_HELPERS_H
#define PIXEL_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void    fatal(char* msg);
char*   join_strings(int n, ...);
char*   to_uri_safe_string(const char* str);

bool    string_starts_with(char *src, size_t src_len, size_t i,  char* frag);

// char* get_duration(clock_t start, clock_t end) {
//     double duration = ((double)(end - start))/CLOCKS_PER_SEC; // in seconds

//     int minutes = (int)duration / 60;
//     int seconds = (int)duration % 60;
//     int milliseconds = ((int)(duration * 1000)) % 1000;
//     int microseconds = ((int)(duration * 1000000)) % 1000;

//     char *formatted_duration = malloc(100 * sizeof(char));
//     if (formatted_duration == NULL) {
//         perror("Failed to allocate memory for formatted_duration");
//         exit(EXIT_FAILURE);
//     }

//     snprintf(formatted_duration, 100, "%dm %ds %dms %dus", 
//              minutes, seconds, milliseconds, microseconds);

//     return formatted_duration;
// }

#endif // PIXEL_HELPERS_H