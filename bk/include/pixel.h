#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define PIXEL_VERSION "0.0.1"

typedef void* Ptr;
typedef struct Ref {
    Ptr     address;
    char*   type;
} Ref; 

typedef enum All_Types {
    DEF_TYPE_NOTHING,

    // Primitives
    DEF_TYPE_INT,
    DEF_TYPE_DEC,

    // Ref Types
    DEF_TYPE_REF_INT,
    DEF_TYPE_REF_DEC,

    // Custom Types
    // CUSTOM_TYPE_POINT

    // Construct this enum at compile time
} All_Types;

// typedef struct Type_Methods {
    // List of Dicts of methods
    // position of the methods corresponds to the All_Types enum
// } Type_Methods;



// Primitive types will be structs to keep them light weight
// 
// Read only Int type
typedef struct Int {
    const All_Types  type;
    const int64_t    value; // value is only available on primitive types
} Int;

// methods for Int can just be stored a normal functions starting with "Int_"

typedef struct Ref_Int {
    const All_Types type;
    const void*     ref_value;
} Ref_Int;

// typedef struct Dec {
//     const Default_Types  type;
//     const double         value;
// } Dec;

// typedef struct Ref_Dec {
//     const Default_Types  type;
//     const void*          ref_value;
// } Ref_Dec;

// Methods for Primitives

typedef struct BaseObject {
    // Public properties
    // Dict*   properties;

    // Public methods
    void (*print) (void* self);

    struct read_only { // Read only properties. Is public
        char* type;
    } read_only;

    struct internal {   // Internal properties. Is private
        void*   address;
        bool    on_heap;
    } internal;

} BaseObject;

// typedef struct Result {
//     bool ok;
//     union
//     {
//         void* value;
//         Error error;
//     };
    
// } Result;

// Define the list structure
#define LIST_DEFAULT_CAPACITY 2
typedef struct List {
    char*   kind;
    Ptr*    items;
    size_t  capacity;
    size_t  length;
} List;

List* list_init(char* kind);
void list_push(List *list, void *item);
Ptr list_pop(List *list);
void list_print(List *list);

// Define the Dict entry structure
#define DICT_INITIAL_CAPACITY 8
#define DICT_LOAD_FACTOR_THRESHOLD 0.75
typedef struct Entry {
    char*           key;
    Ptr             value;
    int             seq; // order of insertion. is eq to dict length at insertion
    struct Entry*   next;
    struct Entry*   prev;
} Entry;

typedef struct Dict {
    Entry** entries;
    size_t  capacity;
    size_t  length;
} Dict;

Dict*   dict_init   (char* kind);
void    dict_push   (Dict* dict, char* key, Ptr value);
Ptr     dict_pop    (Dict* dict, char* key);
Ptr     dict_get    (Dict* dict, char* key);
void    dict_set    (Dict* dict, char* key, Ptr value);
Dict*   dict_seq    (Dict* dict);

#endif // PIXEL_H