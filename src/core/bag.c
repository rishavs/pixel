#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR_THRESHOLD 0.75
#define MAX_KEY_LENGTH 128

// A bag is a lightweight dictionary that stores key-value pairs
// Mainly meant for storing object properties

typedef void* Ptr;
typedef struct Entry {
    char        key[MAX_KEY_LENGTH];
    uint8_t     key_length;
    void*       value;
    struct      Entry* next;
} Entry;

typedef struct Bag {
    char*    type;
    Entry**  entries;
    size_t   capacity;
    size_t   length;
} Bag;

// Hash function - FNV-1a
static uint32_t hash(const char* key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; key[i] != '\0' && i < MAX_KEY_LENGTH; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

static void Entry_set_key(Entry* entry, const char* key) {
    size_t key_length = strlen(key);
    if (key_length >= MAX_KEY_LENGTH) {
        key_length = MAX_KEY_LENGTH - 1;  // Truncate if too long
    }
    memcpy(entry->key, key, key_length);
    entry->key[key_length] = '\0';  // Ensure null-termination
    entry->key_length = key_length;
}

static const char* Entry_get_key(const Entry* entry) {
    return entry->key;
}

Bag* Bag_init(char* type) {
    Bag* bag = (Bag*)malloc(sizeof(Bag));
    if (!bag) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    bag->type = type;
    bag->capacity = INITIAL_CAPACITY;
    bag->length = 0;
    bag->entries = (Entry**)calloc(bag->capacity, sizeof(Entry*));
    if (!bag->entries) {
        fprintf(stderr, "Memory allocation failed\n");
        free(bag);
        return NULL;
    }
    return bag;
}

static void Bag_resize(Bag* bag) {
    size_t new_capacity = bag->capacity * 2;
    Entry** new_entries = (Entry**)calloc(new_capacity, sizeof(Entry*));
    if (!new_entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    for (size_t i = 0; i < bag->capacity; i++) {
        Entry* entry = bag->entries[i];
        while (entry) {
            Entry* next = entry->next;
            size_t index = hash(Entry_get_key(entry)) % new_capacity;
            entry->next = new_entries[index];
            new_entries[index] = entry;
            entry = next;
        }
    }
    
    free(bag->entries);
    bag->entries = new_entries;
    bag->capacity = new_capacity;
}

void Bag_push(Bag* bag, const char* key, Ptr value) {
    if ((float)bag->length / bag->capacity >= LOAD_FACTOR_THRESHOLD) {
        Bag_resize(bag);
    }
    
    size_t index = hash(key) % bag->capacity;
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    if (!new_entry) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    Entry_set_key(new_entry, key);
    new_entry->value = value;
    new_entry->next = bag->entries[index];
    bag->entries[index] = new_entry;
    bag->length++;
}

Entry* Bag_search(Bag* bag, const char* key) {
    size_t index = hash(key) % bag->capacity;
    Entry* entry = bag->entries[index];
    while (entry) {
        if (strcmp(Entry_get_key(entry), key) == 0) {
            return entry;  // Found, return the entry
        }
        entry = entry->next;
    }
    return NULL;  // Not found
}

Ptr Bag_get(Bag* bag, const char* key) {
    Entry* entry = Bag_search(bag, key);
    return entry ? entry->value : NULL;
}

void Bag_set(Bag* bag, const char* key, Ptr value) {
    Entry* entry = Bag_search(bag, key);
    if (entry) {
        entry->value = value;
    } else {
        Bag_push(bag, key, value);
    }
}

void Bag_free(Bag* bag) {
    for (size_t i = 0; i < bag->capacity; i++) {
        Entry* entry = bag->entries[i];
        while (entry) {
            Entry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(bag->entries);
    free(bag);
}

// // Example usage
// int main() {
//     Bag* bag = Bag_init("int");
    
//     int a = 10, b = 20;
//     char* c = "Yo yo ma";
    
//     Bag_push(bag, "one", &a);
//     Bag_push(bag, "two", &b);
//     Bag_push(bag, "three", c);
    
//     printf("Search 'two': %p\n", (void*)Bag_search(bag, "two"));
//     printf("Search 'four': %p\n", (void*)Bag_search(bag, "four"));
    
//     printf("two: %d\n", *(int*)Bag_get(bag, "two"));

//     printf("three: %s\n", (char*)Bag_get(bag, "three"));
    
//     int d = 40;
//     Bag_set(bag, "two", &d);
    
//     printf("two (after update): %d\n", *(int*)Bag_get(bag, "two"));
    
//     Bag_free(bag);
//     return 0;
// }