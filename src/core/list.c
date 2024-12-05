#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "pixel.h"

// Initialize the list
List list_init(size_t item_size) {
    List list;
    list.items = malloc(LIST_DEFAULT_CAPACITY * item_size);
    if (!list.items) fatal_memory_allocation_failure(__FILE__, __LINE__);
    list.item_size = item_size;
    list.capacity = LIST_DEFAULT_CAPACITY;
    list.length = 0;
    return list;
}

// Free the list and its items
void list_free(List *list) {
    free(list->items);
    list->items = NULL;
    list->length = 0;
    list->capacity = 0;
    list->item_size = 0;
}

// Resize the list
static void list_resize(List *list) {
    size_t new_capacity = list->capacity * 2;
    void *new_items = realloc(list->items, new_capacity * list->item_size);
    if (!new_items) fatal_memory_allocation_failure(__FILE__, __LINE__);
    list->items = new_items;
    list->capacity = new_capacity;
}

// Insert an item into the list
void list_push(List *list, const void *item) {
    if (list->length == list->capacity) {
        list_resize(list);
    }
    // Copy the item into the list
    memcpy((char *)list->items + (list->length * list->item_size), item, list->item_size);
    list->length++;
}

// Remove and return the last item in the list
int list_pop(List *list, void *out_item) {
    if (list->length == 0) {
        fprintf(stderr, "List is empty, cannot pop\n");
        return 0;
    }
    list->length--;
    if (out_item) {
        memcpy(out_item, (char *)list->items + (list->length * list->item_size), list->item_size);
    }
    return 1;
}

// Get an item from the list by index
int list_get(const List *list, size_t index, void *out_item) {
    if (index >= list->length) {
        fprintf(stderr, "Index out of bounds\n");
        return 0;
    }
    if (out_item) {
        memcpy(out_item, (char *)list->items + (index * list->item_size), list->item_size);
    }
    return 1;
}

// Set an item in the list at a specific index
int list_set(List *list, size_t index, const void *item) {
    if (index >= list->length) {
        fprintf(stderr, "Index out of bounds\n");
        return 0;
    }
    memcpy((char *)list->items + (index * list->item_size), item, list->item_size);
    return 1;
}

// // Print the list (for debugging)
// void list_print(const List *list, void (*print_item)(const void *)) {
//     printf("List (length: %zu, capacity: %zu): [ ", list->length, list->capacity);
//     for (size_t i = 0; i < list->length; i++) {
//         print_item((char *)list->items + (i * list->item_size));
//         if (i < list->length - 1) {
//             printf(", ");
//         }
//     }
//     printf(" ]\n");
// }
