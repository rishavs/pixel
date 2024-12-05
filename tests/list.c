#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "tests.h"
#include "pixel.h"


Test_Result list_test_01() {
    Test_Result res = { 
        .desc = "A list of integers can push, pop, get &, set",
        .passed = false 
    };

    List list = list_init(sizeof(int));

    // Push some items
    int item1 = 10;
    int item2 = 20;
    int item3 = 30;
    int item4 = 40;

    list_push(&list, &item1);
    list_push(&list, &item2);
    list_push(&list, &item3);
    list_push(&list, &item4);

    int popped_item;
    list_pop(&list, &popped_item);

    int new_item = 70;
    list_set(&list, 1, &new_item);

    int item_at_index_1;
    list_get(&list, 1, &item_at_index_1);

    int item_at_index_2;
    list_get(&list, 2, &item_at_index_2);

    res.passed = list.length == 3 && list.capacity == 8 &&
        popped_item == 40 && list.items != NULL &&
        item_at_index_1 == 70 && 
        item_at_index_2 == 30 ;
        // item_at_index_3 == 40 && 
        // item_at_index_4 == 0;

    list_free(&list);

    return res;
}

Test_Result list_test_02() {
    Test_Result res = { 
        .desc = "A list of strings can push, pop, get &, set",
        .passed = false
    };

    List list = list_init(sizeof(char *));
    char *item1 = "Hello";
    char *item2 = "World";
    char *item3 = "Pixel";
    char *item4 = "Language";

    list_push(&list, &item1);
    list_push(&list, &item2);
    list_push(&list, &item3);
    list_push(&list, &item4);

    char *popped_item;
    list_pop(&list, &popped_item);

    char *new_item = "PixelLang";
    list_set(&list, 1, &new_item);

    char *item_at_index_1;
    list_get(&list, 1, &item_at_index_1);

    char *item_at_index_2;
    list_get(&list, 2, &item_at_index_2);

    res.passed = list.length == 3 && list.capacity == 8 &&
        strcmp(popped_item, "Language") == 0 && list.items != NULL &&
        strcmp(item_at_index_1, "PixelLang") == 0 && 
        strcmp(item_at_index_2, "Pixel") == 0;

    list_free(&list);

    return res;
}


Test_Result list_test_03() {
    Test_Result res = { 
        .desc = "A list of structs can push, pop, get &, set",
        .passed = false
    };

    struct Point {
        int x;
        int y;
    };

    List list = list_init(sizeof(struct Point));
    struct Point item1 = { .x = 10, .y = 20 };
    struct Point item2 = { .x = 30, .y = 40 };
    struct Point item3 = { .x = 50, .y = 60 };
    struct Point item4 = { .x = 70, .y = 80 };

    list_push(&list, &item1);
    list_push(&list, &item2);
    list_push(&list, &item3);
    list_push(&list, &item4);

    struct Point popped_item;
    list_pop(&list, &popped_item);

    struct Point new_item = { .x = 100, .y = 200 };
    list_set(&list, 1, &new_item);

    struct Point item_at_index_1;
    list_get(&list, 1, &item_at_index_1);

    struct Point item_at_index_2;
    list_get(&list, 2, &item_at_index_2);

    res.passed = list.length == 3 && list.capacity == 8 &&
        popped_item.x == 70 && popped_item.y == 80 && list.items != NULL &&
        item_at_index_1.x == 100 && item_at_index_1.y == 200 && 
        item_at_index_2.x == 50 && item_at_index_2.y == 60;

    list_free(&list);

    return res;
}

Test_Result list_test_04() {
    Test_Result res = { 
        .desc = "A list of references to structs can push, pop, get &, set",
        .passed = false
    };

    struct Point {
        int x;
        int y;
    };
    // define a type for the pointer to the struct
    typedef struct Point* Point_Ptr;

    List list = list_init(sizeof(Point_Ptr));
    struct Point item1 = { .x = 10, .y = 20 };
    struct Point item2 = { .x = 30, .y = 40 };
    struct Point item3 = { .x = 50, .y = 60 };
    struct Point item4 = { .x = 70, .y = 80 };

    Point_Ptr item1_ptr = &item1;
    Point_Ptr item2_ptr = &item2;
    Point_Ptr item3_ptr = &item3;
    Point_Ptr item4_ptr = &item4;

    list_push(&list, &item1_ptr);
    list_push(&list, &item2_ptr);
    list_push(&list, &item3_ptr);
    list_push(&list, &item4_ptr);

    Point_Ptr popped_item;
    list_pop(&list, &popped_item);

    struct Point new_item = { .x = 100, .y = 200 };
    Point_Ptr new_item_ptr = &new_item;
    list_set(&list, 1, &new_item_ptr);

    // change the vlue of an item in the list
    item3.x = 300;
    item3.y = 400;

    Point_Ptr item_at_index_0;
    list_get(&list, 0, &item_at_index_0);

    Point_Ptr item_at_index_1;
    list_get(&list, 1, &item_at_index_1);

    Point_Ptr item_at_index_2;
    list_get(&list, 2, &item_at_index_2);

    res.passed = list.length == 3 && list.capacity == 8 && list.items != NULL &&
        popped_item->x == 70 && popped_item->y == 80 && 
        item_at_index_0->x == 10 && item_at_index_0->y == 20 &&
        item_at_index_1->x == 100 && item_at_index_1->y == 200 &&
        item_at_index_2->x == 300 && item_at_index_2->y == 400;

    // Free the list
    list_free(&list);
    return res;
}


Test_Result list_test_05() {
    Test_Result res = { 
        .desc = "A list can grow dynamically",
        .passed = false 
    };

    int items_count = 100;

    List list = list_init(sizeof(int));
    for (int i = 0; i < items_count; i++) {
        list_push(&list, &i);
    }
    
    int first_item;
    list_get(&list, 0, &first_item);

    int last_item;
    list_get(&list, items_count - 1, &last_item);

    res.passed = list.length == items_count && list.capacity == 128 &&
        first_item == 0 && last_item == items_count - 1;

    list_free(&list);

    return res;

}