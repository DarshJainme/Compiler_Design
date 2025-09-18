// Test file 5: Invalid expressions and operators
#include <stdio.h>

int main() {
    // ERROR: Invalid assignment
    int x = ;

    // ERROR: Invalid arithmetic
    int y = 5 + + 3;

    // ERROR: Invalid pointer operation
    int z = *;

    // ERROR: Invalid array access
    int arr[5];
    int val = arr[];

    // ERROR: Invalid function call
    printf(;

    // ERROR: Invalid increment
    int a = 10;
    ++++;

    // ERROR: Invalid comparison
    if (x == ) {
        printf("Invalid comparison\n");
    }

    // ERROR: Invalid logical operation
    if (x && ) {
        printf("Invalid logical\n");
    }

    // ERROR: Unmatched parentheses
    int result = (5 + 3;

    // ERROR: Invalid operator sequence
    int bad = 5 */ 3;

    return 0;
}