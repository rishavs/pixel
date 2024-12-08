#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "errors.h"
#include "pixel.h"

// Macro to define a generic dynamic list type
#define DEFINE_LIST(type, name)                             \
    typedef struct {                                         \
        type *data;                                          \
        size_t count;                                        \
        size_t capacity;                                     \
    } List_of_##name##_t;                                   \
                                                             \
    static inline void List_of_##name##_do_init(List_of_##name##_t* list, size_t initial_capacity) { \
        list->data = NULL;                                  \
        list->count = 0;                                    \
        list->capacity = initial_capacity ? initial_capacity : LIST_DEFAULT_CAPACITY; \
        list->data = calloc(list->capacity, sizeof(type)); \
        if (!list->data) fatal_memory_allocation_failure(__FILE__, __LINE__); \
    }                                                        \
                                                             \
    static inline void List_of_##name##_do_free(List_of_##name##_t* list) { \
        free(list->data);                                   \
        list->data = NULL;                                  \
        list->count = 0;                                    \
        list->capacity = 0;                                 \
    }                                                        \
                                                             \
    static inline void List_of_##name##_do_push(List_of_##name##_t* list, type item) { \
        if (list->count == list->capacity) {               \
            list->capacity = list->capacity > 0 ? list->capacity * 2 : 1; \
            type *new_data = realloc(list->data, list->capacity * sizeof(type)); \
            if (!new_data) fatal_memory_allocation_failure(__FILE__, __LINE__); \
            list->data = new_data;                          \
        }                                                    \
        list->data[list->count++] = item;                  \
    }                                                        \
                                                             \
    static inline bool List_of_##name##_do_pop(List_of_##name##_t* list, type *out_item) { \
        if (list->count == 0) {                             \
            return false;                                    \
        }                                                    \
        *out_item = list->data[--list->count];             \
        return true;                                         \
    }                                                        \
                                                             \
    static inline bool List_of_##name##_do_get(List_of_##name##_t *list, size_t index, type *out_item) { \
        if (index >= list->count) {                         \
            return false;                                    \
        }                                                    \
        *out_item = list->data[index];                      \
        return true;                                         \
    }                                                        \
                                                             \
    static inline bool List_of_##name##_do_set(List_of_##name##_t *list, size_t index, type item) { \
        if (index >= list->count) {                         \
            return false;                                    \
        }                                                    \
        list->data[index] = item;                           \
        return true;                                         \
    }                                                   \
                                                                \
    static inline size_t List_of_##name##_do_count(List_of_##name##_t *list) { \
        return list->count;                                                         \
    }                                                                               \

#endif // LIST_H