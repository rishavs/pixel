#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_THRESHOLD 0.75

typedef void* Ptr;

// Define the Dict entry structure
typedef struct Entry {
    char*           key;
    Ptr             value;
    int             seq; // order of insertion. is eq to dict length at insertion
    struct Entry*   next;
    struct Entry*   prev;
} Entry;

typedef struct Dict {
    Entry** entries;
    size_t     capacity;
    size_t     length;
    Entry*  head;  // For maintaining insertion order
    Entry*  tail;
} Dict;

// Hash function
static uint32_t hash(const char* key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; key[i] != '\0'; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

Dict* dict_init(char* kind) {
    Dict* dict = malloc(sizeof(Dict));
    if (!dict) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    dict->entries = calloc(INITIAL_CAPACITY, sizeof(Entry*));
    if (!dict->entries) {
        fprintf(stderr, "Memory allocation failed\n");
        free(dict);
        return NULL;
    }
    dict->capacity = INITIAL_CAPACITY;
    dict->length = 0;
    dict->head = NULL;
    dict->tail = NULL;
    return dict;
}

static void dict_resize(Dict* dict) {
    int new_capacity = dict->capacity * 2;
    Entry** new_entries = calloc(new_capacity, sizeof(Entry*));
    if (!new_entries) {
        fprintf(stderr, "Memory allocation failed during resize\n");
        return;
    }

    for (int i = 0; i < dict->capacity; i++) {
        Entry* entry = dict->entries[i];
        while (entry) {
            Entry* next = entry->next;
            int index = hash(entry->key) % new_capacity;
            entry->next = new_entries[index];
            new_entries[index] = entry;
            entry = next;
        }
    }

    free(dict->entries);
    dict->entries = new_entries;
    dict->capacity = new_capacity;
}

void dict_push(Dict* dict, char* key, Ptr value) {
    if ((float)dict->length / dict->capacity >= LOAD_FACTOR_THRESHOLD) {
        dict_resize(dict);
    }

    uint32_t index = hash(key) % dict->capacity;
    Entry* new_entry = malloc(sizeof(Entry));
    if (!new_entry) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->seq = dict->length;
    new_entry->next = dict->entries[index];
    new_entry->prev = dict->tail;

    if (dict->tail) {
        dict->tail->next = new_entry;
    } else {
        dict->head = new_entry;
    }
    dict->tail = new_entry;

    dict->entries[index] = new_entry;
    dict->length++;
}

Ptr dict_pop(Dict* dict, char* key) {
    uint32_t index = hash(key) % dict->capacity;
    Entry* entry = dict->entries[index];
    Entry* prev = NULL;

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                dict->entries[index] = entry->next;
            }

            if (entry->prev) {
                entry->prev->next = entry->next;
            } else {
                dict->head = entry->next;
            }

            if (entry->next) {
                entry->next->prev = entry->prev;
            } else {
                dict->tail = entry->prev;
            }

            Ptr value = entry->value;
            free(entry->key);
            free(entry);
            dict->length--;
            return value;
        }
        prev = entry;
        entry = entry->next;
    }

    return NULL;
}

Ptr dict_get(Dict* dict, char* key) {
    uint32_t index = hash(key) % dict->capacity;
    Entry* entry = dict->entries[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

void dict_set(Dict* dict, char* key, Ptr value) {
    uint32_t index = hash(key) % dict->capacity;
    Entry* entry = dict->entries[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    // If key doesn't exist, add it
    dict_push(dict, key, value);
}

Dict* dict_seq(Dict* dict) {
    Dict* seq_dict = dict_init("seq");
    if (!seq_dict) return NULL;

    Entry* current = dict->head;
    while (current) {
        dict_push(seq_dict, current->key, current->value);
        current = current->next;
    }

    return seq_dict;
}

void dict_free(Dict* dict) {
    for (int i = 0; i < dict->capacity; i++) {
        Entry* entry = dict->entries[i];
        while (entry) {
            Entry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(dict->entries);
    free(dict);
}

// Helper function to print the dictionary (for debugging)
void dict_print(Dict* dict) {
    printf("Dictionary (length: %d, capacity: %d):\n", dict->length, dict->capacity);
    Entry* current = dict->head;
    while (current) {
        printf("  %s: %p (seq: %d)\n", current->key, current->value, current->seq);
        current = current->next;
    }
}