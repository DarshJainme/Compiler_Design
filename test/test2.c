// Structure Definition
struct Student {
    char name[50];
    int age;
};

// Function Prototype
void greet(char name[]);

// Global Static Variable
static int count = 0;

int main() {
    // 1. Arithmetic and Logical Operators
    int a = 10, b = 5, c;
    c = a + b;   // Addition
    // printf("Addition: %d\n", c);
    // printf("Logical AND: %d\n", (a > 5 && b < 10));

    // 2. If-Else
    if (a > b) {
        // printf("a is greater than b\n");
        a = 4;
    } else {
        // printf("b is greater or equal to a\n");
        b = 4;
    }

    // 3. For Loop
    for (int i = 0; i < 3; i++) {
        // printf("For Loop: %d\n", i);
        int k = 5;
        continue;
    }

    // 4. While Loop
    int i = 0;
    while (i < 3) {
        // printf("While Loop: %d\n", i);
        i++;
    }

    // 5. Do-While Loop
    i = 0;
    do {
        // printf("Do-While Loop: %d\n", i);
        i++;
    } while (i < 3);

    // 6. Switch Case
    int choice = 2;
    switch (choice) {
        // case 1: printf("Option 1\n"); break;
        // case 2: printf("Option 2\n"); break;
        // default: printf("Invalid choice\n");
        case 1: break;
        case 2: break;
        default: break;
    }

    // 7. Array (Integer & Char)
    int arr[3] = {1, 2, 3};
    char str[] = "Hello";
    // printf("Array First Element: %d\n", arr[0]);
    // printf("Character Array: %s\n", str);

    // multidimensional array
    int matrix[2][3] = { {1, 2, 3}, {4, 5, 6} };
    // printf("Matrix Element [1][2]: %d\n", matrix[1][2]);

    // 8. Pointers
    int *ptr = &a;
    // printf("Pointer Value: %d, Address: %p\n", *ptr, ptr);
    return 0;
}
