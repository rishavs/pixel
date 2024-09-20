#include "../include/greatest.h"

TEST test_list_create() {
    list_t *list = list_create();
    ASSERT_EQ(list->size, 0);
    ASSERT_EQ(list->head, NULL);
    ASSERT_EQ(list->tail, NULL);
    list_destroy(list);
    PASS();
}