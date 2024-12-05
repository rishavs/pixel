#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "errors.h"
#include "tests.h"

// import tests
#include "collections.c"
#include "basic.c"

// Create an array of the tests
Test_Fun tests[] = {
    // Basic tests
    test_01,
    test_02,
    test_03,
    
    // Collections tests
    test_array_01,
    test_array_02,
    test_array_03,
    NULL
};

int main() {
    size_t i = 0;
    size_t passed_count = 0;
    while (tests[i] != NULL) {
        Test_Result res = tests[i]();
        if (res.passed) {
            printf("\033[0;32m  [ PASSED ] %s\n\033[0m", res.desc); // Green color for PASS
            passed_count++;
        } else {
            printf("\033[0;31m  [ FAILED ] %s\n\033[0m", res.desc);
        }
        i++;
    }
    printf("--------------------------------\n");
    printf("Total: %zu, Passed: %zu, Failed: %zu \n", i, passed_count, i-passed_count);
    return 0;
}
