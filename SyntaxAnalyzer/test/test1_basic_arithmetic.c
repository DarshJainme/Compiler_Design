
/* Test Case 1: Basic Arithmetic Operations */
#include <stdio.h>

int main() {
    int x = 10;
    int y = 5;
    int result;
    
    result = x + y;
    result = x - y;
    result = x * y;
    result = x / y;
    result = x % y;
    
    // Assignment operators
    x += 5;
    x -= 3;
    x *= 2;
    x /= 4;
    
    // Increment/decrement
    x++;
    ++x;
    y--;
    --y;
    
    return 0;
}
