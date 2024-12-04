#ifndef PIXEL_TESTS_H
#define PIXEL_TESTS_H

typedef struct {
    const char *name;
    bool is_todo;
    bool (*fun)(void);
} Test;

typedef struct {
    const char *name;
    Test *tests;
} Suite;

bool test_01();
bool test_02();
bool test_03();
// bool test_04();
// bool test_05();
// bool test_06();
// bool test_07();

Suite all_tests = {
    "Test Suite",
    (Test[]) {
        {"Null input", false, test_01},
        {"Only comments and spaces", false, test_02},
        {"Basic variable declaration for an integer", false, test_03},


        {NULL, false, NULL} // NULL entry to terminate the array
    }
};



#endif
