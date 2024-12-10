#ifndef PIXEL_RESOURCES_H
#define PIXEL_RESOURCES_H

// TODO - add verification step to ensure that the number of resources in the enum matches the number of resources in the string array
typedef enum {
    // special content
    RES_ERROR_LOCATION,
    RES_INTERNAL_PERROR,
    RES_INTERNAL_LOCATION,

    // Error Categories
    RES_MEMORY_FAILURE_CAT,
    RES_UNHANDLED_ERROR_CAT,
    RES_SYNTAX_ERROR_CAT,

    // Memory Allocation Failure
    RES_MEMORY_FAILURE_MSG,
    RES_MEMORY_FAILURE_NOTE,
    
    // Unhandled Failures
    RES_UNHANDLED_ERROR_MSG,
    RES_UNHANDLED_ERROR_NOTE,
    
    // Error messages
    RES_ILLEGAL_CHAR_MSG,
    RES_UNCLOSED_DELIMITER_MSG,
    RES_MISSING_STATEMENT_MSG,
    RES_MISSING_EXPRESSION_MSG,
    RES_MISSING_SYNTAX_MSG,
    RES_MISSING_TOKEN_MSG,

    RES_RESOURCE_COUNT // This helps to keep track of the number of error resources
} Error_resource;

// Generate the string array using the macro
static const char* en_us[RES_RESOURCE_COUNT] = {
    [RES_ERROR_LOCATION]        = "at %zu:%zu in the file \"%s\"",
    [RES_INTERNAL_PERROR]       = "Internal Note",
    [RES_INTERNAL_LOCATION]     = "Internal Note: Failed in the compiler code at line %zu in the file \"%s\"",
    
    [RES_MEMORY_FAILURE_CAT]    = "[ FATAL ] Memory Failure!",
    [RES_UNHANDLED_ERROR_CAT]   = "[ FATAL ] Unhandled Failure!",
    [RES_SYNTAX_ERROR_CAT]      = "[ ERROR ] Malformed Syntax!",

    [RES_MEMORY_FAILURE_MSG]    = "Unable to allocate memory for compilation",
    [RES_MEMORY_FAILURE_NOTE]   = "Note: Your device might have run out of memory. Please close other programs and try again.",
    
    [RES_UNHANDLED_ERROR_MSG]   = "Found an unhandled error during compilation ",
    [RES_UNHANDLED_ERROR_NOTE]  = "Note: This is likely not a bug in your code, but in the compiler! Please report with minimal reproducible code.",
    
    [RES_ILLEGAL_CHAR_MSG]      = "Found illegal character \"%c\", ",
    [RES_UNCLOSED_DELIMITER_MSG] = "Found unclosed delimiter \"%s\", ",
    [RES_MISSING_STATEMENT_MSG]    = "Expected a statement, but instead found \"%s\", ",
    [RES_MISSING_EXPRESSION_MSG]    = "Expected an expression, but instead found \"%s\", ",
    [RES_MISSING_SYNTAX_MSG]    = "Expected \"%s\", but instead found \"%s\", ",
    [RES_MISSING_TOKEN_MSG]     = "Expected \"%s\", but instead found \"%s\", ",
};

// const char* error_message = en_us_error_resources[RES_MEMORY_FAILURE_MSG];
// printf("%s\n", error_message); // Output: Memory Allocation Failed!

#endif // PIXEL_RESOURCES_H