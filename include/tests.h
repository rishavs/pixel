#ifndef PIXEL_TESTS_H
#define PIXEL_TESTS_H

#include <stdbool.h>

typedef struct Test_Result {
    char* desc;
    bool passed;
} Test_Result;

// Define the function pointer type
typedef Test_Result (*Test_Fun)();

Test_Result test_01();


#endif
