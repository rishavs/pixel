#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_THRESHOLD 0.75

typedef void* Ptr;

typedef struct Entry {
    char*           key;
    void*           value;
    struct Entry*   next;
} Entry;

typedef struct Bag {
    char*       type;
    Entry**    entries;
    size_t     capacity;
    size_t     length;
} Bag;

// Hash function - FNV-1a
static uint32_t hash(const char* key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; key[i] != '\0'; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

Bag* Bag_init(char* type) {
    Bag* Bag = (Bag*)malloc(sizeof(Bag));
    if (!Bag) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    Bag->type = type;
    Bag->capacity = INITIAL_CAPACITY;
    Bag->length = 0;
    Bag->entries = (Entry**)calloc(Bag->capacity, sizeof(Entry*));
    if (!Bag->entries) {
        fprintf(stderr, "Memory allocation failed\n");
        free(Bag);
        return NULL;
    }
    return Bag;
}

static void Bag_resize(Bag* Bag) {
    size_t new_capacity = Bag->capacity * 2;
    Entry** new_entries = (Entry**)calloc(new_capacity, sizeof(Entry*));
    if (!new_entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    for (size_t i = 0; i < Bag->capacity; i++) {
        Entry* entry = Bag->entries[i];
        while (entry) {
            Entry* next = entry->next;
            size_t index = hash(entry->key) % new_capacity;
            entry->next = new_entries[index];
            new_entries[index] = entry;
            entry = next;
        }
    }
    
    free(Bag->entries);
    Bag->entries = new_entries;
    Bag->capacity = new_capacity;
}

void Bag_push(Bag* Bag, char* key, Ptr value) {
    if ((float)Bag->length / Bag->capacity >= LOAD_FACTOR_THRESHOLD) {
        Bag_resize(Bag);
    }
    
    size_t index = hash(key) % Bag->capacity;
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = Bag->entries[index];
    Bag->entries[index] = new_entry;
    Bag->length++;
}

size_t Bag_search(Bag* Bag, char* key) {
    size_t index = hash(key) % Bag->capacity;
    Entry* entry = Bag->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return index + 1;  // Found, return index + 1
        }
        entry = entry->next;
    }
    return 0;  // Not found
}

Ptr Bag_get(Bag* Bag, char* key) {
    size_t search_result = Bag_search(Bag, key);
    if (search_result == 0) {
        return NULL;  // Key not found
    }
    size_t index = search_result - 1;
    Entry* entry = Bag->entries[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;  // This should never happen if Bag_search worked correctly
}

void Bag_set(Bag* Bag, char* key, Ptr value) {
    size_t search_result = Bag_search(Bag, key);
    if (search_result == 0) {
        // Key doesn't exist, so we'll add it
        Bag_push(Bag, key, value);
    } else {
        // Key exists, update its value
        size_t index = search_result - 1;
        Entry* entry = Bag->entries[index];
        while (entry) {
            if (strcmp(entry->key, key) == 0) {
                entry->value = value;
                break;
            }
            entry = entry->next;
        }
    }
}

// Helper function to free the Bagionary
void Bag_free(Bag* Bag) {
    for (size_t i = 0; i < Bag->capacity; i++) {
        Entry* entry = Bag->entries[i];
        while (entry) {
            Entry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(Bag->entries);
    free(Bag);
}

// Example usage
int main() {
    Bag* Bag = Bag_init("int");
    
    int a = 10, b = 20, c = 30;
    
    Bag_push(Bag, "one", &a);
    Bag_push(Bag, "two", &b);
    Bag_push(Bag, "three", &c);
    
    printf("Search 'two': %zu\n", Bag_search(Bag, "two"));
    printf("Search 'four': %zu\n", Bag_search(Bag, "four"));
    
    printf("two: %d\n", *(int*)Bag_get(Bag, "two"));
    
    int d = 40;
    Bag_set(Bag, "two", &d);
    
    printf("two (after update): %d\n", *(int*)Bag_get(Bag, "two"));
    
    Bag_free(Bag);
    return 0;
}