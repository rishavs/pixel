#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "tests.h"
#include "array.h"
#include "hashmap.h"


// Define an array of integers
DEFINE_ARRAY(int, ints)

// Define an array of strings
DEFINE_ARRAY(char*, strings)

// Define an array of points
typedef struct {
    int x;
    int y;
} Point;
DEFINE_ARRAY(Point, points)

// Test a simple int array
Test_Result test_array_01() {
    Test_Result res = { 
        .desc = "An int array can push and pop items",
        .passed = false 
    };

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

    res.passed = my_array.count == 7 &&
        my_array.capacity == 8 &&
        my_array.data[0] == 10 &&
        my_array.data[1] == 20 &&
        my_array.data[2] == 30 &&
        my_array.data[3] == 40 &&
        my_array.data[4] == 50 &&
        my_array.data[5] == 60 &&
        my_array.data[6] == 70;

    return res;
}

// Test a simple string array
Test_Result test_array_02() {
    Test_Result res = { 
        .desc = "A string array can push and pop items",
        .passed = false 
    };
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

    res.passed = my_array.count == 4 &&
        my_array.capacity == 8 &&
        strcmp(my_array.data[0], "Hello") == 0 &&
        strcmp(my_array.data[1], "World") == 0 &&
        strcmp(my_array.data[2], "Pixel") == 0 &&
        strcmp(my_array.data[3], "Language") == 0;

    return res;
}

// Test a simple struct array
Test_Result test_array_03() {
    Test_Result res = { 
        .desc = "A struct array can push and pop items",
        .passed = false 
    };
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

    points_array_pop(&my_array);

    res.passed = my_array.count == 3 &&
        my_array.capacity == 4 &&
        my_array.data[0].x == 10 &&
        my_array.data[0].y == 20 &&
        my_array.data[1].x == 30 &&
        my_array.data[1].y == 40 &&
        my_array.data[2].x == 50 &&
        my_array.data[2].y == 60;

    return res;
}
