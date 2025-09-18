
/* Test Case: Missing Delimiters - Should produce syntax errors */
#include <stdio.h>

int main() {
    int x = 10  // ERROR: Missing semicolon
    int y = 5;
    
    if (x > y {  // ERROR: Missing closing parenthesis
        printf("x is greater\n");
    }
    
    // ERROR: Missing closing brace
    for (int i = 0; i < 5; i++) {
        printf("i = %d\n", i);
    // Missing closing brace
    
    return 0;
}
