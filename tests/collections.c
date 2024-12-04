#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "array.h"
#include "hashmap.h"

// Define an array of integers
DEFINE_ARRAY(int, ints)

// Define an array of strings
DEFINE_ARRAY(char*, strings)

// define a example struct
typedef struct {
    int x;
    int y;
} Point;

// Define an array of points
DEFINE_ARRAY(Point, points)

// test a simple int array
bool test_array_01() {
    ints_array_t my_array;

    // Initialize with an initial capacity of 4
    ints_array_init(&my_array, 4);

    // Push some items
    ints_array_push(&my_array, 10);
    ints_array_push(&my_array, 20);
    ints_array_push(&my_array, 30);
    ints_array_push(&my_array, 40);
    ints_array_push(&my_array, 50);
    ints_array_push(&my_array, 60);
    ints_array_push(&my_array, 70);

    return my_array.size == 7 &&
        my_array.capacity == 8 &&
        my_array.data[0] == 10 &&
        my_array.data[1] == 20 &&
        my_array.data[2] == 30 &&
        my_array.data[3] == 40 &&
        my_array.data[4] == 50 &&
        my_array.data[5] == 60 &&
        my_array.data[6] == 70;
}

// test a simple string array
bool test_array_02() {
    strings_array_t my_array;

    // Initialize with an initial capacity of 2
    strings_array_init(&my_array, 2);

    // Push some items
    strings_array_push(&my_array, "Hello");
    strings_array_push(&my_array, "World");
    strings_array_push(&my_array, "Pixel");
    strings_array_push(&my_array, "Language");
    strings_array_push(&my_array, "FOO");
    strings_array_push(&my_array, "BAR");

    strings_array_pop(&my_array);
    strings_array_pop(&my_array);

    return my_array.size == 4 &&
        my_array.capacity == 8 &&
        strcmp(my_array.data[0], "Hello") == 0 &&
        strcmp(my_array.data[1], "World") == 0 &&
        strcmp(my_array.data[2], "Pixel") == 0 &&
        strcmp(my_array.data[3], "Language") == 0;
}

// test a simple struct array
bool test_array_03() {
    points_array_t my_array;

    // Initialize with an initial capacity of 2
    points_array_init(&my_array, 2);

    // Push some items
    Point p1 = {10, 20};
    Point p2 = {30, 40};
    Point p3 = {50, 60};
    Point p4 = {70, 80};

    points_array_push(&my_array, p1);
    points_array_push(&my_array, p2);
    points_array_push(&my_array, p3);
    points_array_push(&my_array, p4);

    return my_array.size == 4 &&
        my_array.capacity == 4 &&
        my_array.data[0].x == 10 &&
        my_array.data[0].y == 20 &&
        my_array.data[1].x == 30 &&
        my_array.data[1].y == 40 &&
        my_array.data[2].x == 50 &&
        my_array.data[2].y == 60 &&
        my_array.data[3].x == 70 &&
        my_array.data[3].y == 80;
}
// // Null input
// bool test_01() {
//     char* src = "";
//     TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
//     ctx->src = src;
//     transpile_file(ctx);
//     char* output = DEFAULT_CFILE_CODE;
//     return (strcmp(ctx->cFileCode, output) == 0);
// }

// // Null input with comments
// bool test_02() {
//     char* input = " \
//     -- This is a single line comment \
//     -[ This is a multi \
//     \
//     line comment \
// ]- ";
//     TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
//     ctx->src = input;
//     transpile_file(ctx);
//     char* output = DEFAULT_CFILE_CODE;
//     return (strcmp(ctx->cFileCode, output) == 0);
// }

// // variable declaration for Integer
// bool test_03() {
//     char* input = "var x = 10";
//     TRANSPILER_CONTEXT* ctx = create_transpiler_context("");
//     ctx->src = input;
//     transpile_file(ctx);
//     char* output = "#include <stdint.h>\
// \
// int main() {\
//     int64_t x = 10;\
//     return 0;\
// }";
//     return (strcmp(ctx->cFileCode, output) == 0);
// }
