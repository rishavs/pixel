PAUSE - starting on parser. have defined ASS. Now need to add the parse_file function. Think about how to add node to ASS?


TODO 
    [ ] skip transpiling if null input
    [] work out a way of namepsacing the types like int
    [] add print functions for int, and then for all primitive types
    [] add dev mode. guard "____" at start of identifier as protected


NOTES
    - STRINGS! we will only use char* strings as a default. If the strings need to be mutated, we will use malloc. If not, we will use static strings like `const char *str = "Hello, world!";`
