#ifndef PIXEL_H
#define PIXEL_H

#define PIXEL_VERSION "0.0.1"

typedef enum Pixel_Primitive_Type {
    NIL,
    INT64,
    FLOAT64,
    STRING,
    CHAR,
    BYTE,
    BOOL,
    IO,
};

typedef enum Pixel_Collection_Type { // needs an input type for completion
    LIST,
    DICT,
};

typedef enum Pixel_Complex_Type {
    FUNCTION,                       // needs a return type for completion
    OBJ,
    ENUM,
};

// Predefined Objects as Types
// Error
// Result - Union of Error & T. with an "ok" member

// isRef = false; is how we will track if a vlue is a reference

// List Implementation
// #define LIST_DEFAULT_CAPACITY 8
// typedef struct List {
//     void *items;         // Pointer to the dynamically allocated items
//     size_t item_size;    // Size of each item in bytes
//     size_t capacity;     // Capacity of the list (number of items it can hold)
//     size_t length;       // Current number of items in the list
// } List;
// List list_init(size_t item_size);   // Initialize the list
// void list_free(List *list);     // Free the list and its items
// void list_push(List *list, const void *item);   // Insert an item into the list
// int list_pop(List *list, void *out_item);   // Remove and return the last item in the list
// int list_get(const List *list, size_t index, void *out_item);   // Get an item from the list by index
// int list_set(List *list, size_t index, const void *item);   // Set an item in the list at a specific index

// // Dictionary Implementation
// #define DICT_INITIAL_CAPACITY 8
// #define DICT_LOAD_FACTOR 0.75
// typedef struct Entry {
//     char *key;              // Dynamically allocated key
//     void *value;            // Dynamically allocated value
//     struct Entry *next;     // For chaining in case of collisions
//     struct Entry *next_seq; // Next in insertion order
// } Entry;
// typedef struct {
//     Entry **buckets;       // Array of bucket pointers
//     size_t capacity;       // Current bucket array size
//     size_t count;         // Number of entries in the dictionary
//     size_t value_size;     // Size of the value being stored
//     Entry *head;           // Head of the insertion-ordered list
//     Entry *tail;           // Tail of the insertion-ordered list
// } Dict;
// Dict dict_init(size_t value_size);  // Initialize the dictionary
// void dict_free(Dict *dict);   // Free all entries and their data
// void dict_set(Dict *dict, const char *key, const void *value);  // Add or update a key-value pair
// int dict_get(Dict *dict, const char *key, void *out_value);  // Retrieve a value by key
// int dict_remove(Dict *dict, const char *key);   // Remove a key-value pair by key

#endif // PIXEL_H