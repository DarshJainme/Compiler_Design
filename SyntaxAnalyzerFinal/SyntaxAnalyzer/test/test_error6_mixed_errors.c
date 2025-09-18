// Test file 6: Mixed syntax errors
#include <stdio.h>

// ERROR: Invalid function declaration
void func1(int, , char c);

// ERROR: Invalid struct with missing semicolon
struct Point {
    int x;
    int y
}

// ERROR: Invalid variable declaration
int main() {
    // ERROR: Undeclared variable
    undeclared_var = 100;

    // ERROR: Invalid array declaration
    int array[;

    // ERROR: Invalid if statement
    if (1 > 0 {
        printf("Missing closing parenthesis\n");
    }

    // ERROR: Invalid while loop
    while (1 > 0) 
        printf("Missing braces\n");
        // This will cause issues

    // ERROR: Invalid for loop
    for (int i = 0; i < 10 i++) {
        printf("%d\n", i);
    }

    // ERROR: Invalid function call
    printf("Hello %d", );

    // ERROR: Invalid return
    return;
    // ERROR: Missing closing brace for main function