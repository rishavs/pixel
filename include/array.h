#ifndef PIXEL_ARRAY_H
#define PIXEL_ARRAY_H

#include <stdlib.h>
#include <stdio.h>

#include "errors.h"

// Macro to define a generic dynamic array type
#define DEFINE_ARRAY(type, name)                             \
    typedef struct {                                         \
        type *data;                                          \
        size_t count;                                         \
        size_t capacity;                                     \
    } name##_array_t;                                        \
                                                             \
    static inline void name##_array_init(name##_array_t *array, size_t initial_capacity) { \
        array->data = NULL;                                  \
        array->count = 0;                                     \
        array->capacity = 0;                                 \
        if (initial_capacity > 0) {                         \
            array->data = malloc(initial_capacity * sizeof(type)); \
            if (!array->data) fatal_memory_allocation_failure(__FILE__, __LINE__); \
            array->capacity = initial_capacity;              \
        }                                                    \
    }                                                        \
                                                            \
    static inline void name##_array_free(name##_array_t *array) { \
        free(array->data);                                   \
        array->data = NULL;                                  \
        array->count = 0;                                     \
        array->capacity = 0;                                 \
    }                                                        \
                                                             \
    static inline void name##_array_push(name##_array_t *array, type item) { \
        if (array->count == array->capacity) {                \
            array->capacity = array->capacity > 0 ? array->capacity * 2 : 1; \
            array->data = realloc(array->data, array->capacity * sizeof(type)); \
            if (!array->data) fatal_memory_allocation_failure(__FILE__, __LINE__); \
        }                                                    \
        array->data[array->count++] = item;                   \
    }                                                        \
                                                             \
    static inline type name##_array_pop(name##_array_t *array) { \
        if (array->count == 0) fatal_memory_allocation_failure(__FILE__, __LINE__); \
        return array->data[--array->count];                   \
    }                                                        \

#endif // PIXEL_ARRAY_H
