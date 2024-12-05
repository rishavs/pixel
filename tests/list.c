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

    int items_at_index_1;
    list_get(&list, 1, &items_at_index_1);

    int items_at_index_2;
    list_get(&list, 2, &items_at_index_2);

    res.passed = list.length == 3 && list.capacity == 8 &&
        popped_item == 40 && list.items != NULL &&
        items_at_index_1 == 70 && 
        items_at_index_2 == 30 && 
        // items_at_index_3 == 40 && 
        // items_at_index_4 == 0;

    list_free(&list);

    return res;
}

Test_Result list_test_02() {
    Test_Result res = { 
        .desc = "A list can get items by index",
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

    int out_item;
    list_get(&list, 2, &out_item);

    res.passed = out_item == 30;

    list_free(&list);

    return res;
}

Test_Result list_test_03() {
    Test_Result res = { 
        .desc = "A list can set items by index",
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

    int new_item = 100;
    list_set(&list, 2, &new_item);

    int out_item;
    list_get(&list, 2, &out_item);

    res.passed = out_item == 100;

    list_free(&list);

    return res;
}

Test_Result list_test_04() {
    Test_Result res = { 
        .desc = "A list can grow dynamically",
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

    res.passed = list.capacity == 4;

    int item5 = 50;
    list_push(&list, &item5);

    res.passed = res.passed && list.capacity == 8;

    list_free(&list);

    return res;
}

Test_Result list_test_05() {
    Test_Result res = { 
        .desc = "A list can shrink dynamically",
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

    res.passed = list.capacity == 4;

    list_pop(&list, NULL);
    list_pop(&list, NULL);
    list_pop(&list, NULL);

    res.passed = res.passed && list.capacity == 2;

    list_free(&list);

    return res;
}