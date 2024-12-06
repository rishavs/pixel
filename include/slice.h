#ifndef SLICE_H
#define SLICE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "errors.h"
#include "pixel.h"

// Macro to define a generic dynamic slice type
#define DEFINE_SLICE(type, name)                             \
    typedef struct {                                         \
        type *data;                                          \
        size_t count;                                        \
        size_t capacity;                                     \
    } Slice_of_##name##_t;                                   \
                                                             \
    static inline void Slice_of_##name##_do_init(Slice_of_##name##_t* slice, size_t initial_capacity) { \
        slice->data = NULL;                                  \
        slice->count = 0;                                    \
        slice->capacity = initial_capacity ? initial_capacity : LIST_DEFAULT_CAPACITY; \
        slice->data = calloc(slice->capacity, sizeof(type)); \
        if (!slice->data) fatal_memory_allocation_failure(__FILE__, __LINE__); \
    }                                                        \
                                                             \
    static inline void Slice_of_##name##_do_free(Slice_of_##name##_t* slice) { \
        free(slice->data);                                   \
        slice->data = NULL;                                  \
        slice->count = 0;                                    \
        slice->capacity = 0;                                 \
    }                                                        \
                                                             \
    static inline void Slice_of_##name##_do_push(Slice_of_##name##_t* slice, type item) { \
        if (slice->count == slice->capacity) {               \
            slice->capacity = slice->capacity > 0 ? slice->capacity * 2 : 1; \
            type *new_data = realloc(slice->data, slice->capacity * sizeof(type)); \
            if (!new_data) fatal_memory_allocation_failure(__FILE__, __LINE__); \
            slice->data = new_data;                          \
        }                                                    \
        slice->data[slice->count++] = item;                  \
    }                                                        \
                                                             \
    static inline bool Slice_of_##name##_do_pop(Slice_of_##name##_t* slice, type *out_item) { \
        if (slice->count == 0) {                             \
            return false;                                    \
        }                                                    \
        *out_item = slice->data[--slice->count];             \
        return true;                                         \
    }                                                        \
                                                             \
    static inline bool Slice_of_##name##_do_get(Slice_of_##name##_t *slice, size_t index, type *out_item) { \
        if (index >= slice->count) {                         \
            return false;                                    \
        }                                                    \
        *out_item = slice->data[index];                      \
        return true;                                         \
    }                                                        \
                                                             \
    static inline bool Slice_of_##name##_do_set(Slice_of_##name##_t *slice, size_t index, type item) { \
        if (index >= slice->count) {                         \
            return false;                                    \
        }                                                    \
        slice->data[index] = item;                           \
        return true;                                         \
    }

#endif // SLICE_H