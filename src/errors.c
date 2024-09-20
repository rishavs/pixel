#include <stdio.h>

#include "pixel.h"

void error_print(Error* error) {
    printf("Error: %s\n", error->type);
    printf("File: %s\n", error->file);
    printf("Header: %s\n", error->header);
    printf("Message: %s\n", error->msg);
    printf("Line: %zu\n", error->line);
    printf("Position: %zu\n", error->pos);
    printf("Fatal: %s\n", error->fatal ? "true" : "false");
}
