
/* Test Case: Invalid Statements - Should produce syntax errors */
#include <stdio.h>

int main() {
    int x = 10;
    
    // ERROR: Invalid statement
    x y z;
    
    // ERROR: Invalid if statement
    if x > 5 {
        printf("Invalid\n");
    }
    
    // ERROR: Invalid for loop
    for i = 0; i < 5; i++ {
        printf("Invalid\n");
    }
    
    // ERROR: Invalid function call
    printf("Hello" "World";  // Missing closing parenthesis
    
    return 0;
}
