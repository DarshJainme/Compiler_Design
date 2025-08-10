#include <stdio.h>

int main() {
    int i;
    for (i = 0; i < 10; i++) {
        if (i == 5) {
            break;
        }
        if (i == 2) {
            continue;
        }
    }
    
    while (i > 0) {
        i--;
    }
    
    do {
        i++;
    } while (i < 5);
    
    switch (i) {
        case 1:
            printf("One");
            break;
        default:
            printf("Other");
    }
    
    return 0;
}