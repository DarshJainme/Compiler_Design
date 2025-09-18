
// Test file 4: Misplaced keywords and invalid syntax
#include <stdio.h>

// ERROR: Missing function body
void incomplete_function();

// ERROR: Invalid variable declaration
int 123invalid = 10;

// ERROR: Misplaced static keyword
static if (1) {
    printf("Invalid static usage\n");
}

// ERROR: Invalid struct syntax
struct {
    int x;
    // ERROR: Missing member name
    float;
} incomplete_struct;

// ERROR: Invalid typedef
typedef struct;

// ERROR: Invalid class declaration (if classes supported)
class {
    // ERROR: Missing class name
    int member;
};

// ERROR: Invalid function definition
return int function() {
    return 0;
}

// ERROR: Misplaced break outside loop
int main() {
    break;
    
    // ERROR: Invalid switch without expression
    switch {
        case 1:
            printf("Invalid switch\n");
            break;
    }
    
    // ERROR: Invalid for loop
    for (int i = 0; ; ) {
        if (i > 10) break;
        i++;
    }
    
    return 0;
}
