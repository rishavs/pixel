#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/pixel.h"

void test_dict_init() {
    Dict *dict = dict_init();
    assert(dict != NULL);
    assert(dict->size == 8);
    assert(dict->count == 0);
    dict_free(dict);
    printf("test_dict_init passed\n");
}

void test_dict_insert_and_get() {
    Dict *dict = dict_init();
    char *value1 = "Value 1";
    char *value2 = "Value 2";
    char *value3 = "Value 3";

    dict_insert(dict, "key1", value1);
    dict_insert(dict, "key2", value2);
    dict_insert(dict, "key3", value3);

    assert(strcmp((char*)dict_get(dict, "key1"), value1) == 0);
    assert(strcmp((char*)dict_get(dict, "key2"), value2) == 0);
    assert(strcmp((char*)dict_get(dict, "key3"), value3) == 0);

    dict_free(dict);
    printf("test_dict_insert_and_get passed\n");
}

void test_dict_set() {
    Dict *dict = dict_init();
    char *value1 = "Value 1";
    char *value2 = "Value 2";
    char *new_value2 = "New Value 2";

    dict_insert(dict, "key1", value1);
    dict_insert(dict, "key2", value2);

    dict_set(dict, "key2", new_value2);
    assert(strcmp((char*)dict_get(dict, "key2"), new_value2) == 0);

    dict_free(dict);
    printf("test_dict_set passed\n");
}

void test_dict_delete() {
    Dict *dict = dict_init();
    char *value1 = "Value 1";
    char *value2 = "Value 2";

    dict_insert(dict, "key1", value1);
    dict_insert(dict, "key2", value2);

    dict_delete(dict, "key2");
    assert(dict_get(dict, "key2") == NULL);

    dict_free(dict);
    printf("test_dict_delete passed\n");
}

void test_dict_resize() {
    Dict *dict = dict_init();
    char *value = "Value";

    for (int i = 0; i < 100; i++) {
        char key[10];
        snprintf(key, sizeof(key), "key%d", i);
        dict_insert(dict, key, value);
    }

    for (int i = 0; i < 100; i++) {
        char key[10];
        snprintf(key, sizeof(key), "key%d", i);
        assert(strcmp((char*)dict_get(dict, key), value) == 0);
    }

    dict_free(dict);
    printf("test_dict_resize passed\n");
}

int main() {
    test_dict_init();
    test_dict_insert_and_get();
    test_dict_set();
    test_dict_delete();
    test_dict_resize();
    printf("All tests passed\n");
    return 0;
}