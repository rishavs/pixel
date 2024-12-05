#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "tests.h"
#include "pixel.h"

// Test dictionary initialization and setting/getting values
Test_Result dict_of_int_can_set_get_remove() {
    Test_Result res = { 
        .desc = "Dictionary: A dictionary of ints can set, get & remove items",
        .passed = false 
    };

    Dict dict = dict_init(sizeof(int));

    // Set some items
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    int value4 = 40;

    dict_set(&dict, "one", &value1);
    dict_set(&dict, "two", &value2);
    dict_set(&dict, "three", &value3);
    dict_set(&dict, "four", &value4);

    int item_one;
    dict_get(&dict, "one", &item_one);

    int item_two;
    dict_get(&dict, "two", &item_two);

    int removed_item_three;
    int item_three;
    dict_remove(&dict, "three");
    dict_get(&dict, "three", &item_three);

    int item_four;
    dict_get(&dict, "four", &item_four);

    res.passed = dict.count == 3 &&
        item_one == 10 && 
        item_two == 20 && 
        !item_three  &&
        item_four == 40;

    dict_free(&dict);

    return res;
}

Test_Result dict_of_string_can_set_get_remove() {
    Test_Result res = { 
        .desc = "Dictionary: A dictionary of strings can set, get & remove items",
        .passed = false 
    };

    Dict dict = dict_init(sizeof(char*));

    // Set some items
    char* value1 = "Hello";
    char* value2 = "World";
    char* value3 = "Pixel";
    char* value4 = "Language";

    dict_set(&dict, "one", &value1);
    dict_set(&dict, "two", &value2);
    dict_set(&dict, "three", &value3);
    dict_set(&dict, "four", &value4);

    char* item_one;
    dict_get(&dict, "one", &item_one);

    char* item_two;
    dict_get(&dict, "two", &item_two);

    char* item_three;
    dict_remove(&dict, "three");
    dict_get(&dict, "three", &item_three);

    char* item_four;
    dict_get(&dict, "four", &item_four);

    res.passed = dict.count == 3 &&
        strcmp(item_one, "Hello") == 0 && 
        strcmp(item_two, "World") == 0 && 
        !item_three  &&
        strcmp(item_four, "Language") == 0;

    dict_free(&dict);

    return res;
}

Test_Result dict_of_structs_can_get_set_remove() {
    Test_Result res = { 
        .desc = "Dictionary: A dictionary of structs can set, get & remove items",
        .passed = false 
    };

    typedef struct {
        int x;
        int y;
    } Point;

    Dict dict = dict_init(sizeof(Point));

    // Set some items
    Point value1 = { .x = 10, .y = 20 };
    Point value2 = { .x = 30, .y = 40 };
    Point value3 = { .x = 50, .y = 60 };
    Point value4 = { .x = 70, .y = 80 };

    dict_set(&dict, "one", &value1);
    dict_set(&dict, "two", &value2);
    dict_set(&dict, "three", &value3);
    dict_set(&dict, "four", &value4);

    Point item_one;
    dict_get(&dict, "one", &item_one);

    Point item_two;
    dict_get(&dict, "two", &item_two);

    Point item_three;
    dict_remove(&dict, "three");
    dict_get(&dict, "three", &item_three);

    Point item_four;
    dict_get(&dict, "four", &item_four);

    res.passed = dict.count == 3 &&
        item_one.x == 10 && item_one.y == 20 && 
        item_two.x == 30 && item_two.y == 40 && 
        !item_three.x && !item_three.y &&
        item_four.x == 70 && item_four.y == 80;

    dict_free(&dict);

    return res;
}

Test_Result dict_of_struct_refs_can_get_set_remove() {
    Test_Result res = { 
        .desc = "Dictionary: A dictionary of struct references can set, get & remove items",
        .passed = false 
    };

    typedef struct {
        int x;
        int y;
    } Point;

    typedef Point* Point_Ptr;

    Dict dict = dict_init(sizeof(Point_Ptr));

    // Set some items
    Point item1 = { .x = 10, .y = 20 };
    Point item2 = { .x = 30, .y = 40 };
    Point item3 = { .x = 50, .y = 60 };
    Point item4 = { .x = 70, .y = 80 };

    Point_Ptr item1_ptr = &item1;
    Point_Ptr item2_ptr = &item2;
    Point_Ptr item3_ptr = &item3;
    Point_Ptr item4_ptr = &item4;

    dict_set(&dict, "one", &item1_ptr);
    dict_set(&dict, "two", &item2_ptr);
    dict_set(&dict, "three", &item3_ptr);
    dict_set(&dict, "four", &item4_ptr);

    Point_Ptr item_one;
    dict_get(&dict, "one", &item_one);

    item2.x = 123;
    item2.y = 456;
    Point_Ptr item_two;
    dict_get(&dict, "two", &item_two);

    Point_Ptr item_three;
    dict_remove(&dict, "three");
    dict_get(&dict, "three", &item_three);

    Point_Ptr item_four;
    dict_get(&dict, "four", &item_four);

    res.passed = dict.count == 3 &&
        item_one->x == 10 && item_one->y == 20 &&
        item_two->x == 123 && item_two->y == 456 && 
        !item_three &&
        item_four->x == 70 && item_four->y == 80;

    dict_free(&dict);

    return res;
}

