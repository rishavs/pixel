#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/tests.h"

int main() {
    printf("Testing Suite: %s\n", all_tests.name);

    size_t total_tests = 0;
    size_t total_passed = 0;
    size_t total_failed = 0;
    size_t total_skipped = 0;

    size_t i = 0;
    while (all_tests.tests[i].name != NULL) {
        total_tests++;
        Test test = all_tests.tests[i];
        if (test.is_todo) {
            total_skipped++;
            printf("\033[0;34m  [ TODO ]\033[0m"); // Blue for TODO
        } else {
            if (test.fun()) {
                total_passed++;
                printf("\033[0;32m  [ PASS ]\033[0m"); // Green color for PASS
            } else {
                total_failed++;
                printf("\033[0;31m  [ FAIL ]\033[0m"); // Red color for FAIL
            }

        }
        printf("    %zu) %s\n", i, test.name);
        i++;
    }

    printf("-------------------------------------------------\n");
    printf("Total tests: %zu\t, Passed: %zu\t, Failed: %zu\t, Skipped: %zu\n", total_tests, total_passed, total_failed, total_skipped);

    return 0;
}