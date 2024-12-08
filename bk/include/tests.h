#ifndef SEAWITCH_TESTS_H
#define SEAWITCH_TESTS_H

typedef struct {
    const char *name;
    bool is_todo;
    bool (*fun)(void);
} Test;

typedef struct {
    const char *name;
    Test *tests;
} Suite;

// Suite all_tests = {
//     "Test Suite",
//     (Test[]) {
//         {"Null input", false, test_01},
//         {"Null input with comments", false, test_02},
//         {"Simple integer", true, test_03},


//         {NULL, false, NULL} // NULL entry to terminate the array
//     }
// };



#endif
