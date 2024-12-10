#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "resources.h"
#include "transpiler.h"

void print_errors (Transpiler_context_t* ctx) {
    if (ctx->errors_count == 0) return;
    fprintf(stderr, "\033[0;31m-------------------------------------------\n");
    fprintf(stderr, "Compilation Failed! Found %zu error(s)\n", ctx->errors_count);
    fprintf(stderr, "-------------------------------------------\n");

    for (size_t i = 0; i < ctx->errors_count; i++) {
        Transpiler_error_t e = ctx->errors[i];
        fprintf(stderr, "%zu:\t%s %s", i, e.category, e.msg);
        fprintf(stderr, en_us[RES_ERROR_LOCATION], e.pos, e.line, e.filepath);
        fprintf(stderr, "\n");
        perror(en_us[RES_INTERNAL_PERROR]);
        fprintf(stderr, en_us[RES_INTERNAL_LOCATION], e.transpiler_line, e.transpiler_file);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\033[0m");

}