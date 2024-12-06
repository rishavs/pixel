#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "errors.h"
#include "tests.h"
#include "pixel.h"

// import tests
// #include "arrays.c"
#include "list.tests.c"
#include "dict.tests.c"
#include "basic.tests.c"
#include "slice.tests.c"
#include "record.tests.c"

// Create an array of the tests
Test_Fun tests[] = {
    // Basic tests
    test_01,
    test_02,
    test_03,
    
    // Array tests
    // test_array_01,
    // test_array_02,
    // test_array_03,

    // // Hashmap tests
    // test_hashmap_01,
    // test_hashmap_02,

    // List tests
    list_test_01,
    list_test_02,
    list_test_03,
    list_test_04,
    list_test_05,

    // Dict tests
    dict_of_int_can_set_get_remove,
    dict_of_string_can_set_get_remove,
    dict_of_structs_can_get_set_remove,
    dict_of_struct_refs_can_get_set_remove,

    // Slice tests
    slice_of_int_can_push_pop_get_set,
    slice_of_strings_can_push_pop_get_set,
    slice_of_points_can_push_pop_get_set,
    slice_of_refs_to_point_can_push_pop_get_set,

    // Record tests
    record_of_int_can_set_get_remove,
    record_of_string_can_set_get_remove,
    record_of_structs_can_get_set_remove,
    record_of_struct_refs_can_get_set_remove,

    // End of tests
    NULL
};

int main() {
    size_t i = 0;
    size_t passed_count = 0;
    while (tests[i] != NULL) {
        Test_Result res = tests[i]();
        if (res.passed) {
            printf("\033[0;32m%zu: \t[ PASSED ]\t%s\n\033[0m", i, res.desc); // Green color for PASS
            passed_count++;
        } else {
            printf("\033[0;31m%zu:\t[ FAILED ]\t%s\n\033[0m", i, res.desc);
        }
        i++;
    }
    printf("--------------------------------\n");
    printf("Total: %zu, Passed: %zu, Failed: %zu \n", i, passed_count, i-passed_count);
    return 0;
}
