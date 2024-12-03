#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pixel.h"

// Initialize the list
List* list_init(char* kind){
    List *list = (List*)malloc(sizeof(List));
    if (!list) perror("Memory allocation failed on list_init");

    list->kind = kind;

    list->items = (Ptr*)calloc(LIST_DEFAULT_CAPACITY, sizeof(Ptr));
    if (!list->items) perror("Memory allocation failed on list_init");

    list->capacity = LIST_DEFAULT_CAPACITY;
    list->length = 0;
    return list;
}

// Resize the list
void list_resize(List *list) {
    size_t new_capacity = list->capacity * 2;
    Ptr* new_items = (Ptr*)realloc(list->items, new_capacity * sizeof(Ptr));
    if (!new_items) perror("Memory allocation failed on list_resize");
    list->items = new_items;
    list->capacity = new_capacity;
}

// Insert an item into the list
void list_push(List *list, Ptr item) {
    if (list->length == list->capacity) {
        list_resize(list);
    }
    list->items[list->length++] = item;
}

Ptr list_pop(List *list) {
    if (list->length == 0) perror("List cannot be popped as it is empty");
    const Ptr item = list->items[list->length - 1];
    list->items[list->length - 1] = NULL;
    list->length--;
    return item;
}

// Get an item from the list by index
Ptr list_get(List *list, size_t index) {
    if (index >= list->length) perror("Index out of bounds");
    return list->items[index];
}

// Set an item in the list by index
void list_set(List *list, size_t index, Ptr item) {
    if (index >= list->length) perror("Index out of bounds");
    list->items[index] = item;
}

void list_print(List *list) {
    printf("List Length: %zu\n", list->length);
    printf("List type: %s\n", list->kind);
    printf("{ ");

    for (size_t i = 0; i < list->length; i++) {
        if (strcmp(list->kind,  "List<Int>") == 0) {
            printf("%d ", *(int*)list->items[i]);
        } else if (strcmp(list->kind, "List<Dec>") == 0) {
            printf("%f ", *(float*)list->items[i]);
        } else if (strcmp(list->kind, "char") == 0) {
            printf("%c ", *(char*)list->items[i]);
        } else if (strcmp(list->kind, "string") == 0) {
            printf("%s ", (char*)list->items[i]);
        } else if (strcmp(list->kind, "ptr") == 0) {
            printf("%p ", list->items[i]);
        } else {
            printf("Unknown type\n");
            break;
        }
    }

    printf(" }\n");

}

