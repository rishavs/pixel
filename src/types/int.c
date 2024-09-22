typedef struct Int Int;
typedef struct Int_Properties {
    char* type;
    const bool is_on_heap;

    char* (*to_string) (void*);
    Int (*new) (int64_t);
} Int_Properties;

// Read only Int type
typedef struct Int {
    // data
    const int64_t    value; // value is only available on primitive types
    
    // Properties
    Int_Properties  properties;

} Int;

char* Int_to_string(Int* self) {
    char* str = malloc(20);
    sprintf(str, "%ld", self->value);
    return str;
}

Int Int_new(int64_t value) {
    Int i = {
        .value = value,
        .properties = {
            .type = "Int",
            .is_on_heap = false,

            .new = Int_new,
            .to_string = Int_to_string
        }
    };
    return i;
}

int main() {
    Int myInt = Int_new(10);
    char* str = myInt.properties.to_string(&myInt);
    printf("Int: %s\n", str);
    free(str); // Free the allocated memory
    return 0;
}