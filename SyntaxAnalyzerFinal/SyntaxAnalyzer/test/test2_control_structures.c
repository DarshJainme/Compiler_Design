
/* Test Case 2: Control Structures */
#include <stdio.h>

int main() {
    int i = 0;
    int x = 10;
    int y = 5;
    
    // If-else statement
    if (x > y) {
        printf("x is greater\n");
    } else {
        printf("y is greater or equal\n");
    }
    
    // While loop
    while (i < 5) {
        printf("i = %d\n", i);
        i++;
    }
    
    // For loop
    for (int j = 0; j < 3; j++) {
        printf("j = %d\n", j);
    }
    
    // Do-while loop
    do {
        printf("Do-while iteration\n");
        i--;
    } while (i > 0);
    
    // Switch statement
    switch (x) {
        case 10:
            printf("x is 10\n");
            break;
        case 5:
            printf("x is 5\n");
            break;
        default:
            printf("x is something else\n");
            break;
    }
    
    return 0;
}
