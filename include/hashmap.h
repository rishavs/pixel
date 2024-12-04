#ifndef PIXEL_HASHMAP_H
#define PIXEL_HASHMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>



// Maximum length for keys
#define MAX_KEY_LENGTH 128

// Macro to define a generic hashmap
#define DEFINE_HASHMAP(value_type, name)                            \
    typedef struct {                                                \
        char key[MAX_KEY_LENGTH];                                   \
        value_type value;                                           \
    } name##_pair_t;                                                \
                                                                    \
    typedef struct {                                                \
        name##_pair_t *data;                                        \
        size_t size;                                                \
        size_t capacity;                                            \
    } name##_hashmap_t;                                             \
                                                                    \
    static inline uint64_t name##_hash(const char *key) {           \
        uint64_t hash = 5381;                                       \
        int c;                                                      \
        while ((c = *key++))                                        \
            hash = ((hash << 5) + hash) + c;                        \
        return hash;                                                \
    }                                                               \
                                                                    \
    static inline void name##_hashmap_init(name##_hashmap_t *map, size_t initial_capacity) { \
        map->data = NULL;                                           \
        map->size = 0;                                              \
        map->capacity = 0;                                          \
        if (initial_capacity > 0) {                                 \
            map->data = calloc(initial_capacity, sizeof(name##_pair_t)); \
            if (!map->data) {                                       \
                fprintf(stderr, "Out of memory\n");                 \
                exit(EXIT_FAILURE);                                 \
            }                                                       \
            map->capacity = initial_capacity;                       \
        }                                                           \
    }                                                               \
                                                                    \
    static inline void name##_hashmap_rehash(name##_hashmap_t *map) { \
        size_t new_capacity = map->capacity > 0 ? map->capacity * 2 : 1; \
        name##_pair_t *new_data = calloc(new_capacity, sizeof(name##_pair_t)); \
        if (!new_data) {                                            \
            fprintf(stderr, "Out of memory\n");                     \
            exit(EXIT_FAILURE);                                     \
        }                                                           \
        for (size_t i = 0; i < map->capacity; i++) {                \
            if (map->data[i].key[0] != '\0') {                      \
                uint64_t hash = name##_hash(map->data[i].key);      \
                size_t idx = hash % new_capacity;                   \
                while (new_data[idx].key[0] != '\0') {              \
                    idx = (idx + 1) % new_capacity;                 \
                }                                                   \
                new_data[idx] = map->data[i];                       \
            }                                                       \
        }                                                           \
        free(map->data);                                            \
        map->data = new_data;                                       \
        map->capacity = new_capacity;                               \
    }                                                               \
                                                                    \
    static inline void name##_hashmap_set(name##_hashmap_t *map, const char *key, value_type value) { \
        if (map->size + 1 > map->capacity / 2) {                    \
            name##_hashmap_rehash(map);                             \
        }                                                           \
        uint64_t hash = name##_hash(key);                           \
        size_t idx = hash % map->capacity;                          \
        while (map->data[idx].key[0] != '\0') {                     \
            if (strncmp(map->data[idx].key, key, MAX_KEY_LENGTH) == 0) { \
                map->data[idx].value = value;                       \
                return;                                             \
            }                                                       \
            idx = (idx + 1) % map->capacity;                        \
        }                                                           \
        strncpy(map->data[idx].key, key, MAX_KEY_LENGTH);           \
        map->data[idx].key[MAX_KEY_LENGTH - 1] = '\0';              \
        map->data[idx].value = value;                               \
        map->size++;                                                \
    }                                                               \
                                                                    \
    static inline int name##_hashmap_get(name##_hashmap_t *map, const char *key, value_type *out_value) { \
        if (map->capacity == 0) return 0;                           \
        uint64_t hash = name##_hash(key);                           \
        size_t idx = hash % map->capacity;                          \
        while (map->data[idx].key[0] != '\0') {                     \
            if (strncmp(map->data[idx].key, key, MAX_KEY_LENGTH) == 0) { \
                *out_value = map->data[idx].value;                  \
                return 1;                                           \
            }                                                       \
            idx = (idx + 1) % map->capacity;                        \
        }                                                           \
        return 0;                                                   \
    }                                                               \
                                                                    \
    static inline int name##_hashmap_remove(name##_hashmap_t *map, const char *key) { \
        if (map->capacity == 0) return 0;                           \
        uint64_t hash = name##_hash(key);                           \
        size_t idx = hash % map->capacity;                          \
        while (map->data[idx].key[0] != '\0') {                     \
            if (strncmp(map->data[idx].key, key, MAX_KEY_LENGTH) == 0) { \
                map->data[idx].key[0] = '\0';                       \
                map->size--;                                        \
                return 1;                                           \
            }                                                       \
            idx = (idx + 1) % map->capacity;                        \
        }                                                           \
        return 0;                                                   \
    }                                                               \

#endif // PIXEL_HASHMAP_H
