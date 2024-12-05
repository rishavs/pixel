#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "pixel.h"

// FNV-1a hash function
static uint32_t hash(const char *key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; key[i] != '\0'; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

// Initialize the dictionary
Dict dict_init(size_t value_size) {
    Dict dict;
    dict.buckets = calloc(DICT_INITIAL_CAPACITY, sizeof(Entry *));
    if (!dict.buckets) {
        perror("Failed to allocate buckets");
        exit(EXIT_FAILURE);
    }
    dict.capacity = DICT_INITIAL_CAPACITY;
    dict.count = 0;
    dict.value_size = value_size;
    dict.head = NULL;
    dict.tail = NULL;
    return dict;
}

// Free all entries and their data
void dict_free(Dict *dict) {
    for (size_t i = 0; i < dict->capacity; i++) {
        Entry *entry = dict->buckets[i];
        while (entry) {
            Entry *next = entry->next;
            free(entry->key);    // Free the key
            free(entry->value);  // Free the value
            free(entry);
            entry = next;
        }
    }
    free(dict->buckets);
}

// Resize the dictionary
static void dict_resize(Dict *dict) {
    size_t new_capacity = dict->capacity * 2;
    Entry **new_buckets = calloc(new_capacity, sizeof(Entry *));
    if (!new_buckets) {
        perror("Failed to allocate new buckets");
        return;
    }

    // Rehash entries into the new bucket array
    for (size_t i = 0; i < dict->capacity; i++) {
        Entry *entry = dict->buckets[i];
        while (entry) {
            Entry *next = entry->next;

            uint32_t index = hash(entry->key) % new_capacity;
            entry->next = new_buckets[index];
            new_buckets[index] = entry;

            entry = next;
        }
    }

    free(dict->buckets);
    dict->buckets = new_buckets;
    dict->capacity = new_capacity;
}

// Add or update a key-value pair
void dict_set(Dict *dict, const char *key, const void *value) {
    if ((float)dict->count / dict->capacity >= DICT_LOAD_FACTOR) {
        dict_resize(dict);
    }

    uint32_t index = hash(key) % dict->capacity;
    Entry *entry = dict->buckets[index];

    // Check if the key already exists
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            memcpy(entry->value, value, dict->value_size); // Update the value
            return;
        }
        entry = entry->next;
    }

    // Create a new entry
    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry) {
        perror("Failed to allocate entry");
        return;
    }
    new_entry->key = strdup(key);
    if (!new_entry->key) {
        perror("Failed to allocate key");
        free(new_entry);
        return;
    }
    new_entry->value = malloc(dict->value_size);
    if (!new_entry->value) {
        perror("Failed to allocate value");
        free(new_entry->key);
        free(new_entry);
        return;
    }
    memcpy(new_entry->value, value, dict->value_size);
    new_entry->next = dict->buckets[index];
    new_entry->next_seq = NULL;

    // Update insertion order
    if (dict->tail) {
        dict->tail->next_seq = new_entry;
    } else {
        dict->head = new_entry;
    }
    dict->tail = new_entry;

    dict->buckets[index] = new_entry;
    dict->count++;
}

// Retrieve a value by key
int dict_get(Dict *dict, const char *key, void *out_value) {
    uint32_t index = hash(key) % dict->capacity;
    Entry *entry = dict->buckets[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            memcpy(out_value, entry->value, dict->value_size);
            return 1; // Found
        }
        entry = entry->next;
    }
    // set the value of out_value to NULL
    memset(out_value, 0, dict->value_size);

    return 0; // Not found
}

// Remove a key-value pair by key
int dict_remove(Dict *dict, const char *key) {
    uint32_t index = hash(key) % dict->capacity;
    Entry *entry = dict->buckets[index];
    Entry *prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                dict->buckets[index] = entry->next;
            }

            // Update insertion order
            if (dict->head == entry) {
                dict->head = entry->next_seq;
            }
            if (dict->tail == entry) {
                dict->tail = NULL;
            }

            free(entry->key);
            free(entry->value);
            free(entry);
            dict->count--;
            return 1; // Removed
        }
        prev = entry;
        entry = entry->next;
    }
    return 0; // Not found
}
