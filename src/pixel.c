#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pixel.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Seawitch Programming Language\n");
        printf("Version: %s\n", PIXEL_VERSION);
        printf("Usage: hexal [options] <file>\n");
        printf("Options:\n");
        printf("  -h, --help\t\tPrint this help message\n");
        printf("  -v, --version\t\tPrint version information\n");
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "run") == 0) {
        printf("Compiling project with entrypoint file: %s\n", argv[2]);
        char* filepath = argv[2];

        // TRANSPILER_CONTEXT* ctx = create_transpiler_context(filepath);
        // validate_file(ctx);
        // read_file(ctx);
        // transpile_file(ctx);
        // build_cfiles(ctx);
        // compile_cfiles(ctx);
        // present outcomes(ctx);
        // execute program
        
        return 0;
    }
    printf("Invalid arguments\n");
    return 1;
}