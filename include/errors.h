#ifndef PIXEL_ERRORS_H
#define PIXEL_ERRORS_H

#include <stdlib.h>
#include <stdio.h>

#include "transpiler.h"

// Fatal failure. Memory allocation failed.
void memory_allocation_failure(size_t pos, size_t line, const char* filepath, char* transpiler_file, size_t transpiler_line);

// Fatal failure. Unhandled error.
void unhandled_error(size_t pos, size_t line, const char* filepath, char* transpiler_file, size_t transpiler_line);




#endif // PIXEL_ERRORS_H