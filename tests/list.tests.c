#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "errors.h"
#include "tests.h"
#include "list.h"


// Define an array of integers
DEFINE_LIST(int, ints)

// Define an array of strings
DEFINE_LIST(char*, strings)

// Define an array of points
typedef struct {
    int x;
    int y;
} Point;
DEFINE_LIST(Point, points)

// Define an array of references to Points
DEFINE_LIST(Point*, refs_to_point)

// Test a simple int array
Test_Result list_of_int_can_push_pop_get_set() {
    Test_Result res = { 
        .desc = "List: list of ints can push and pop items",
        .passed = false 
    };

    List_of_ints_t my_list;

    // Initialize with an initial capacity of 4
    List_of_ints_do_init(&my_list, 4);

    // Push some items
    List_of_ints_do_push(&my_list, 10);
    List_of_ints_do_push(&my_list, 20);
    List_of_ints_do_push(&my_list, 30);
    List_of_ints_do_push(&my_list, 40);
    List_of_ints_do_push(&my_list, 50);
    List_of_ints_do_push(&my_list, 60);
    List_of_ints_do_push(&my_list, 70);

    int popped_item;
    List_of_ints_do_pop(&my_list, &popped_item);

    bool set_item_1 = List_of_ints_do_set(&my_list, 5, 555);

    int get_item_0;
    List_of_ints_do_get(&my_list, 0, &get_item_0);
    int get_item_5;
    List_of_ints_do_get(&my_list, 5, &get_item_5);
    int get_item_6;
    List_of_ints_do_get(&my_list, 6, &get_item_6);

    res.passed = my_list.count == 6 && my_list.capacity == 8 &&
        popped_item == 70 &&
        get_item_0 == 10 &&
        get_item_5 == 555 && set_item_1 &&
        !get_item_6;

    List_of_ints_do_free(&my_list);

    return res;
}

// Test a simple string array
Test_Result list_of_strings_can_push_pop_get_set() {
    Test_Result res = { 
        .desc = "List: list of strings can push, pop, get & set items",
        .passed = false 
    };

    List_of_strings_t my_list;

    // Initialize with an initial capacity of 4
    List_of_strings_do_init(&my_list, 4);

    // Push some items
    List_of_strings_do_push(&my_list, "Hello");
    List_of_strings_do_push(&my_list, "World");
    List_of_strings_do_push(&my_list, "Pixel");
    List_of_strings_do_push(&my_list, "Language");
    List_of_strings_do_push(&my_list, "is");
    List_of_strings_do_push(&my_list, "Awesome");

    char* popped_item;
    List_of_strings_do_pop(&my_list, &popped_item);

    char* new_item = "My";
    bool new_item_res = List_of_strings_do_set(&my_list, 2, new_item);

    char* item_at_index_1;
    bool item_at_index_1_res = List_of_strings_do_get(&my_list, 1, &item_at_index_1);

    char* item_at_index_2;
    bool item_at_index_2_res = List_of_strings_do_get(&my_list, 2, &item_at_index_2);

    char* item_at_index_5;
    bool item_at_index_5_res = List_of_strings_do_get(&my_list, 5, &item_at_index_5);

    res.passed = my_list.count == 5 && my_list.capacity == 8 &&
        strcmp(popped_item, "Awesome") == 0 &&
        item_at_index_1_res && strcmp(item_at_index_1, "World") == 0 && 
        new_item_res && item_at_index_2_res && strcmp(item_at_index_2, "My") == 0 &&
        // item_at_index_5 == "";

    List_of_strings_do_free(&my_list);

    return res;
}

// Test a simple struct array
Test_Result list_of_points_can_push_pop_get_set() {
    Test_Result res = { 
        .desc = "List: list of points can push, pop, get & set items",
        .passed = false 
    };

    List_of_points_t my_list;

    // Initialize with an initial capacity of 4
    List_of_points_do_init(&my_list, 4);

    // Push some items
    Point item0 = { .x = 10, .y = 20 };
    Point item1 = { .x = 30, .y = 40 };
    Point item2 = { .x = 50, .y = 60 };
    Point item3 = { .x = 70, .y = 80 };

    List_of_points_do_push(&my_list, item0);
    List_of_points_do_push(&my_list, item1);
    List_of_points_do_push(&my_list, item2);
    List_of_points_do_push(&my_list, item3);

    Point popped_item;
    bool popped_item_res = List_of_points_do_pop(&my_list, &popped_item);

    Point new_item = { .x = 100, .y = 200 };
    bool new_item_res = List_of_points_do_set(&my_list, 0, new_item);

    Point item_at_index_0;
    bool get_item_0_res = List_of_points_do_get(&my_list, 0, &item_at_index_0);

    Point item_at_index_2;
    bool get_item_2 = List_of_points_do_get(&my_list, 2, &item_at_index_2);

    Point item_at_index_3;
    bool get_item_3 = List_of_points_do_get(&my_list, 3, &item_at_index_3);

    res.passed = my_list.count == 3 && my_list.capacity == 4 &&
        popped_item_res && popped_item.x == 70 && popped_item.y == 80 &&
        get_item_0_res && item_at_index_0.x == 100 && item_at_index_0.y == 200 && 
        get_item_2 && item_at_index_2.x == 50 && item_at_index_2.y == 60 &&
        !get_item_3;

    List_of_points_do_free(&my_list);

    return res;
}

// Test a list made of references to Points
Test_Result list_of_refs_to_point_can_push_pop_get_set() {
    Test_Result res = { 
        .desc = "List: list of references to points can push, pop, get & set items",
        .passed = false 
    };

    List_of_refs_to_point_t my_list;

    // Initialize with an initial capacity of 4
    List_of_refs_to_point_do_init(&my_list, 4);

    // Push some items
    Point item0 = { .x = 10, .y = 20 };
    Point item1 = { .x = 30, .y = 40 };
    Point item2 = { .x = 50, .y = 60 };
    Point item3 = { .x = 70, .y = 80 };

    Point* item0_ptr = &item0;
    Point* item1_ptr = &item1;
    Point* item2_ptr = &item2;
    Point* item3_ptr = &item3;

    List_of_refs_to_point_do_push(&my_list, item0_ptr);
    List_of_refs_to_point_do_push(&my_list, item1_ptr);
    List_of_refs_to_point_do_push(&my_list, item2_ptr);
    List_of_refs_to_point_do_push(&my_list, item3_ptr);

    Point* popped_item;
    bool possed_item_res = List_of_refs_to_point_do_pop(&my_list, &popped_item);

    Point new_item = { .x = 100, .y = 200 };
    Point* new_item_ptr = &new_item;
    bool new_item_ptr_res = List_of_refs_to_point_do_set(&my_list, 0, new_item_ptr);

    // change the vlue of an item outside of the list
    item1.x = 300;
    item1.y = 400;

    Point* item_at_index_0;
    bool item_at_index_0_res = List_of_refs_to_point_do_get(&my_list, 0, &item_at_index_0);

    Point* item_at_index_1;
    bool item_at_index_1_res = List_of_refs_to_point_do_get(&my_list, 1, &item_at_index_1);

    Point* item_at_index_2;
    bool item_at_index_2_res = List_of_refs_to_point_do_get(&my_list, 2, &item_at_index_2);

    Point* item_at_index_3;
    bool item_at_index_3_res = List_of_refs_to_point_do_get(&my_list, 3, &item_at_index_3);

    res.passed = my_list.count == 3 && my_list.capacity == 4 && my_list.data != NULL &&
        popped_item->x == 70 && popped_item->y == 80 && 
        new_item_ptr_res && item_at_index_0_res && item_at_index_0->x == 100 && item_at_index_0->y == 200 &&
        item_at_index_1_res && item_at_index_1->x == 300 && item_at_index_1->y == 400 &&
        item_at_index_2_res && item_at_index_2->x == 50 && item_at_index_2->y == 60 &&
        !item_at_index_3_res;

    List_of_refs_to_point_do_free(&my_list);

    return res;
}