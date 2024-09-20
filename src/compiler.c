// #include <stdio.h>
// #include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "pixel.h"
#include "compiler.h"

// void free_tokens(ListOfTokens* tokens) {
//     c_foreach(i, ListOfTokens, *tokens) {
//         cstr_drop(&i.ref->kind);
//         cstr_drop(&i.ref->value);
//     }
//     ListOfTokens_drop(tokens);
// }

void compile_file (char* filepath) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    bool ok = false;
    List* errors = list_init("List<Error>");

    char* src;
    ok = read_file (errors, &src, filepath);
    if (!ok) {
        printf("Failed to read file\n");
        return;
    }
    printf("Source code:\n---------------------------------\n");
    printf("%s", src);


    List* tokens = list_init("List<Token>");
    if (!tokens) perror("Memory allocation failed on list_init");



//     ListOfTokens tokens = lex_file(src);
//     printf("Tokens:\n---------------------------------\n");
//     c_foreach (i, ListOfTokens, tokens)  {   // Alternative and recommended way to iterate.
//         printf("%s: %s, pos %lli, line %zu \n", cstr_str(&i.ref->kind), cstr_str(&i.ref->value), i.ref->pos, i.ref->line);      // i.ref is a pointer to the current element.
//     }

//     int parsed = parse_file(tokens);
    
//     cstr_drop(&src);
//     free_tokens(&tokens);
        
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("\n---------------------------------\n");
    printf("Compilation took %f ms to execute \n", cpu_time_used);        
}

// create an type a;ias for int
// typedef int myint;