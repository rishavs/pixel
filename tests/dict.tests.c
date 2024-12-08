#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "errors.h"
#include "tests.h"
#include "dict.h"


// Define a dict of integers
DEFINE_DICT(int, ints)

// Define a dict of strings
DEFINE_DICT(char*, strings)

// Define a dict of points
// typedef struct {
//     int x;
//     int y;
// } Point;
DEFINE_DICT(Point, points)

// Define a dict of references to Points
DEFINE_DICT(Point*, refs_to_point)

// Test a simple int dict
Test_Result dict_of_int_can_set_get_remove() {
    Test_Result res = { 
        .desc = "Dict: dict of ints can set, get and remove items",
        .passed = false 
    };

    Dict_of_ints_t my_dict;

    // Initialize with an initial capacity of 4
    Dict_of_ints_do_init(&my_dict, 4);

    // Set some items
    Dict_of_ints_do_set(&my_dict, "one", 1);
    Dict_of_ints_do_set(&my_dict, "two", 2);
    Dict_of_ints_do_set(&my_dict, "three", 3);
    Dict_of_ints_do_set(&my_dict, "four", 4);
    Dict_of_ints_do_set(&my_dict, "five", 5);
    Dict_of_ints_do_set(&my_dict, "six", 6);
    Dict_of_ints_do_set(&my_dict, "seven", 7);

    int get_one;
    bool get_one_res = Dict_of_ints_do_get(&my_dict, "one", &get_one);
    int get_six;
    bool get_six_res = Dict_of_ints_do_get(&my_dict, "six", &get_six);
    int get_seven;
    bool get_seven_res = Dict_of_ints_do_get(&my_dict, "seven", &get_seven);

    bool remove_one = Dict_of_ints_do_remove(&my_dict, "one");
    bool remove_six = Dict_of_ints_do_remove(&my_dict, "six");
    bool remove_seven = Dict_of_ints_do_remove(&my_dict, "seven");

    int get_one_after_remove;
    bool get_one_after_remove_res = Dict_of_ints_do_get(&my_dict, "one", &get_one_after_remove);
    int get_six_after_remove;
    bool get_six_after_remove_res = Dict_of_ints_do_get(&my_dict, "six", &get_six_after_remove);
    int get_seven_after_remove;
    bool get_seven_after_remove_res = Dict_of_ints_do_get(&my_dict, "seven", &get_seven_after_remove);

    res.passed = my_dict.count == 4 &&
        get_one_res && get_one == 1 &&
        get_six_res && get_six == 6 &&
        get_seven_res && get_seven == 7 &&
        remove_one && !get_one_after_remove_res &&
        remove_six && !get_six_after_remove_res &&
        remove_seven && !get_seven_after_remove_res;

    Dict_of_ints_do_free(&my_dict);

    return res;
}

Test_Result dict_of_string_can_set_get_remove() {
    Test_Result res = { 
        .desc = "Dict: dict of strings can set, get and remove items",
        .passed = false 
    };

    Dict_of_strings_t my_dict;

    // Initialize with an initial capacity of 4
    Dict_of_strings_do_init(&my_dict, 4);

    // Set some items
    Dict_of_strings_do_set(&my_dict, "one", "One");
    Dict_of_strings_do_set(&my_dict, "two", "Two");
    Dict_of_strings_do_set(&my_dict, "three", "Three");
    Dict_of_strings_do_set(&my_dict, "four", "Four");
    Dict_of_strings_do_set(&my_dict, "five", "Five");
    Dict_of_strings_do_set(&my_dict, "six", "Six");
    Dict_of_strings_do_set(&my_dict, "seven", "Seven");

    char* get_one;
    bool get_one_res = Dict_of_strings_do_get(&my_dict, "one", &get_one);
    char* get_six;
    bool get_six_res = Dict_of_strings_do_get(&my_dict, "six", &get_six);
    char* get_seven;
    bool get_seven_res = Dict_of_strings_do_get(&my_dict, "seven", &get_seven);

    bool remove_one = Dict_of_strings_do_remove(&my_dict, "one");
    bool remove_six = Dict_of_strings_do_remove(&my_dict, "six");
    bool remove_seven = Dict_of_strings_do_remove(&my_dict, "seven");

    char* get_one_after_remove;
    bool get_one_after_remove_res = Dict_of_strings_do_get(&my_dict, "one", &get_one_after_remove);
    char* get_six_after_remove;
    bool get_six_after_remove_res = Dict_of_strings_do_get(&my_dict, "six", &get_six_after_remove);
    char* get_seven_after_remove;
    bool get_seven_after_remove_res = Dict_of_strings_do_get(&my_dict, "seven", &get_seven_after_remove);

    res.passed = my_dict.count == 4 &&
        get_one_res && strcmp(get_one, "One") == 0 &&
        get_six_res && strcmp(get_six, "Six") == 0 &&
        get_seven_res && strcmp(get_seven, "Seven") == 0 &&
        remove_one && !get_one_after_remove_res &&
        remove_six && !get_six_after_remove_res &&
        remove_seven && !get_seven_after_remove_res;

    Dict_of_strings_do_free(&my_dict);

    return res;
}

Test_Result dict_of_structs_can_get_set_remove() {
    Test_Result res = { 
        .desc = "Dict: dict of structs can set, get and remove items",
        .passed = false 
    };

    Dict_of_points_t my_dict;

    // Initialize with an initial capacity of 4
    Dict_of_points_do_init(&my_dict, 4);

    // Set some items
    Point item0 = { .x = 10, .y = 20 };
    Point item1 = { .x = 30, .y = 40 };
    Point item2 = { .x = 50, .y = 60 };
    Point item3 = { .x = 70, .y = 80 };

    Dict_of_points_do_set(&my_dict, "one", item0);
    Dict_of_points_do_set(&my_dict, "two", item1);
    Dict_of_points_do_set(&my_dict, "three", item2);
    Dict_of_points_do_set(&my_dict, "four", item3);

    Point get_one;
    bool get_one_res = Dict_of_points_do_get(&my_dict, "one", &get_one);
    Point get_three;
    bool get_three_res = Dict_of_points_do_get(&my_dict, "three", &get_three);

    bool remove_one = Dict_of_points_do_remove(&my_dict, "one");
    bool remove_three = Dict_of_points_do_remove(&my_dict, "three");

    Point get_one_after_remove;
    bool get_one_after_remove_res = Dict_of_points_do_get(&my_dict, "one", &get_one_after_remove);
    Point get_three_after_remove;
    bool get_three_after_remove_res = Dict_of_points_do_get(&my_dict, "three", &get_three_after_remove);

    res.passed = my_dict.count == 2 &&
        get_one_res && get_one.x == 10 && get_one.y == 20 &&
        get_three_res && get_three.x == 50 && get_three.y == 60 &&
        remove_one && !get_one_after_remove_res &&
        remove_three && !get_three_after_remove_res;

    Dict_of_points_do_free(&my_dict);

    return res;
}

// Test a dict made of references to Points
Test_Result dict_of_struct_refs_can_get_set_remove() {
    Test_Result res = { 
        .desc = "Dict: dict of references to points can set, get and remove items",
        .passed = false 
    };

    Dict_of_refs_to_point_t my_dict;

    // Initialize with an initial capacity of 4
    Dict_of_refs_to_point_do_init(&my_dict, 4);

    // Set some items
    Point item0 = { .x = 10, .y = 20 };
    Point item1 = { .x = 30, .y = 40 };
    Point item2 = { .x = 50, .y = 60 };
    Point item3 = { .x = 70, .y = 80 };

    Point* item0_ptr = &item0;
    Point* item1_ptr = &item1;
    Point* item2_ptr = &item2;
    Point* item3_ptr = &item3;

    Dict_of_refs_to_point_do_set(&my_dict, "one", item0_ptr);
    Dict_of_refs_to_point_do_set(&my_dict, "two", item1_ptr);
    Dict_of_refs_to_point_do_set(&my_dict, "three", item2_ptr);
    Dict_of_refs_to_point_do_set(&my_dict, "four", item3_ptr);

    bool remove_one = Dict_of_refs_to_point_do_remove(&my_dict, "one");

    item2.x = 500;
    item2.y = 600;

    Point* get_one_after_remove;
    bool get_one_after_remove_res = Dict_of_refs_to_point_do_get(&my_dict, "one", &get_one_after_remove);
    Point* get_two;
    bool get_two_res = Dict_of_refs_to_point_do_get(&my_dict, "two", &get_two);

    Point* get_three_after_edit;
    bool get_three_after_edit_res = Dict_of_refs_to_point_do_get(&my_dict, "three", &get_three_after_edit);

    res.passed = my_dict.count == 3 &&
        !get_one_after_remove &&
        get_two_res && get_two->x == 30 && get_two->y == 40 &&
        get_three_after_edit_res && get_three_after_edit->x == 500 && get_three_after_edit->y == 600;
        
    Dict_of_refs_to_point_do_free(&my_dict);

    return res;
}

// // Define an array of integers
// DEFINE_SLICE(int, ints)

// // Define an array of strings
// DEFINE_SLICE(char*, strings)

// // Define an array of points
// typedef struct {
//     int x;
//     int y;
// } Point;
// DEFINE_SLICE(Point, points)

// // Define an array of references to Points
// DEFINE_SLICE(Point*, refs_to_point)

// // Test a simple int array
// Test_Result slice_of_int_can_push_pop_get_set() {
//     Test_Result res = { 
//         .desc = "Slice: slice of ints can push and pop items",
//         .passed = false 
//     };

//     Slice_of_ints_t my_slice;

//     // Initialize with an initial capacity of 4
//     Slice_of_ints_do_init(&my_slice, 4);

//     // Push some items
//     Slice_of_ints_do_push(&my_slice, 10);
//     Slice_of_ints_do_push(&my_slice, 20);
//     Slice_of_ints_do_push(&my_slice, 30);
//     Slice_of_ints_do_push(&my_slice, 40);
//     Slice_of_ints_do_push(&my_slice, 50);
//     Slice_of_ints_do_push(&my_slice, 60);
//     Slice_of_ints_do_push(&my_slice, 70);

//     int popped_item;
//     Slice_of_ints_do_pop(&my_slice, &popped_item);

//     bool set_item_1 = Slice_of_ints_do_set(&my_slice, 5, 555);

//     int get_item_0;
//     Slice_of_ints_do_get(&my_slice, 0, &get_item_0);
//     int get_item_5;
//     Slice_of_ints_do_get(&my_slice, 5, &get_item_5);
//     int get_item_6;
//     Slice_of_ints_do_get(&my_slice, 6, &get_item_6);

//     res.passed = my_slice.count == 6 && my_slice.capacity == 8 &&
//         popped_item == 70 &&
//         get_item_0 == 10 &&
//         get_item_5 == 555 && set_item_1 &&
//         !get_item_6;

//     Slice_of_ints_do_free(&my_slice);

//     return res;
// }

// // Test a simple string array
// Test_Result slice_of_strings_can_push_pop_get_set() {
//     Test_Result res = { 
//         .desc = "Slice: slice of strings can push, pop, get & set items",
//         .passed = false 
//     };

//     Slice_of_strings_t my_slice;

//     // Initialize with an initial capacity of 4
//     Slice_of_strings_do_init(&my_slice, 4);

//     // Push some items
//     Slice_of_strings_do_push(&my_slice, "Hello");
//     Slice_of_strings_do_push(&my_slice, "World");
//     Slice_of_strings_do_push(&my_slice, "Pixel");
//     Slice_of_strings_do_push(&my_slice, "Language");
//     Slice_of_strings_do_push(&my_slice, "is");
//     Slice_of_strings_do_push(&my_slice, "Awesome");

//     char* popped_item;
//     Slice_of_strings_do_pop(&my_slice, &popped_item);

//     char* new_item = "My";
//     bool new_item_res = Slice_of_strings_do_set(&my_slice, 2, new_item);

//     char* item_at_index_1;
//     bool item_at_index_1_res = Slice_of_strings_do_get(&my_slice, 1, &item_at_index_1);

//     char* item_at_index_2;
//     bool item_at_index_2_res = Slice_of_strings_do_get(&my_slice, 2, &item_at_index_2);

//     char* item_at_index_5;
//     bool item_at_index_5_res = Slice_of_strings_do_get(&my_slice, 5, &item_at_index_5);

//     res.passed = my_slice.count == 5 && my_slice.capacity == 8 &&
//         strcmp(popped_item, "Awesome") == 0 &&
//         item_at_index_1_res && strcmp(item_at_index_1, "World") == 0 && 
//         new_item_res && item_at_index_2_res && strcmp(item_at_index_2, "My") == 0 &&
//         // item_at_index_5 == "";

//     Slice_of_strings_do_free(&my_slice);

//     return res;
// }

// // Test a simple struct array
// Test_Result slice_of_points_can_push_pop_get_set() {
//     Test_Result res = { 
//         .desc = "Slice: slice of points can push, pop, get & set items",
//         .passed = false 
//     };

//     Slice_of_points_t my_slice;

//     // Initialize with an initial capacity of 4
//     Slice_of_points_do_init(&my_slice, 4);

//     // Push some items
//     Point item0 = { .x = 10, .y = 20 };
//     Point item1 = { .x = 30, .y = 40 };
//     Point item2 = { .x = 50, .y = 60 };
//     Point item3 = { .x = 70, .y = 80 };

//     Slice_of_points_do_push(&my_slice, item0);
//     Slice_of_points_do_push(&my_slice, item1);
//     Slice_of_points_do_push(&my_slice, item2);
//     Slice_of_points_do_push(&my_slice, item3);

//     Point popped_item;
//     bool popped_item_res = Slice_of_points_do_pop(&my_slice, &popped_item);

//     Point new_item = { .x = 100, .y = 200 };
//     bool new_item_res = Slice_of_points_do_set(&my_slice, 0, new_item);

//     Point item_at_index_0;
//     bool get_item_0_res = Slice_of_points_do_get(&my_slice, 0, &item_at_index_0);

//     Point item_at_index_2;
//     bool get_item_2 = Slice_of_points_do_get(&my_slice, 2, &item_at_index_2);

//     Point item_at_index_3;
//     bool get_item_3 = Slice_of_points_do_get(&my_slice, 3, &item_at_index_3);

//     res.passed = my_slice.count == 3 && my_slice.capacity == 4 &&
//         popped_item_res && popped_item.x == 70 && popped_item.y == 80 &&
//         get_item_0_res && item_at_index_0.x == 100 && item_at_index_0.y == 200 && 
//         get_item_2 && item_at_index_2.x == 50 && item_at_index_2.y == 60 &&
//         !get_item_3;

//     Slice_of_points_do_free(&my_slice);

//     return res;
// }

// // Test a slice made of references to Points
// Test_Result slice_of_refs_to_point_can_push_pop_get_set() {
//     Test_Result res = { 
//         .desc = "Slice: slice of references to points can push, pop, get & set items",
//         .passed = false 
//     };

//     Slice_of_refs_to_point_t my_slice;

//     // Initialize with an initial capacity of 4
//     Slice_of_refs_to_point_do_init(&my_slice, 4);

//     // Push some items
//     Point item0 = { .x = 10, .y = 20 };
//     Point item1 = { .x = 30, .y = 40 };
//     Point item2 = { .x = 50, .y = 60 };
//     Point item3 = { .x = 70, .y = 80 };

//     Point* item0_ptr = &item0;
//     Point* item1_ptr = &item1;
//     Point* item2_ptr = &item2;
//     Point* item3_ptr = &item3;

//     Slice_of_refs_to_point_do_push(&my_slice, item0_ptr);
//     Slice_of_refs_to_point_do_push(&my_slice, item1_ptr);
//     Slice_of_refs_to_point_do_push(&my_slice, item2_ptr);
//     Slice_of_refs_to_point_do_push(&my_slice, item3_ptr);

//     Point* popped_item;
//     bool possed_item_res = Slice_of_refs_to_point_do_pop(&my_slice, &popped_item);

//     Point new_item = { .x = 100, .y = 200 };
//     Point* new_item_ptr = &new_item;
//     bool new_item_ptr_res = Slice_of_refs_to_point_do_set(&my_slice, 0, new_item_ptr);

//     // change the vlue of an item outside of the list
//     item1.x = 300;
//     item1.y = 400;

//     Point* item_at_index_0;
//     bool item_at_index_0_res = Slice_of_refs_to_point_do_get(&my_slice, 0, &item_at_index_0);

//     Point* item_at_index_1;
//     bool item_at_index_1_res = Slice_of_refs_to_point_do_get(&my_slice, 1, &item_at_index_1);

//     Point* item_at_index_2;
//     bool item_at_index_2_res = Slice_of_refs_to_point_do_get(&my_slice, 2, &item_at_index_2);

//     Point* item_at_index_3;
//     bool item_at_index_3_res = Slice_of_refs_to_point_do_get(&my_slice, 3, &item_at_index_3);

//     res.passed = my_slice.count == 3 && my_slice.capacity == 4 && my_slice.data != NULL &&
//         popped_item->x == 70 && popped_item->y == 80 && 
//         new_item_ptr_res && item_at_index_0_res && item_at_index_0->x == 100 && item_at_index_0->y == 200 &&
//         item_at_index_1_res && item_at_index_1->x == 300 && item_at_index_1->y == 400 &&
//         item_at_index_2_res && item_at_index_2->x == 50 && item_at_index_2->y == 60 &&
//         !item_at_index_3_res;

//     Slice_of_refs_to_point_do_free(&my_slice);

//     return res;
// }