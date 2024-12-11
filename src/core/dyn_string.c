

typedef struct {
    char* data;
    size_t len;
    size_t capacity;
} Dyn_string_t;

Dyn_string_t* dyn_string_do_init() {
    Dyn_string_t str;
    str.len = 0;
    str.capacity = 8;
    str.data = calloc(str.capacity, sizeof(char));
    if (str.data == NULL) {
        perror("Failed to allocate memory for string");
        exit(EXIT_FAILURE);
    }

    return &str
}

Dyn_string_t* dyn_string_do_push(Dyn_string_t* str, char* frag) {
    size_t frag_len = strlen(frag);
    if (str->len + frag_len >= str->capacity) {
        str->capacity *= 2;
        str->data = realloc(str->data, str->capacity * sizeof(char));
        if (str->data == NULL) {
            perror("Failed to reallocate memory for string");
            exit(EXIT_FAILURE);
        }
    }

    strncpy(str->data + str->len, frag, frag_len);
    str->len += frag_len;
    str->data[str->len] = '\0';

    return str->data;
}