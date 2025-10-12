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
    printf("Addition: %d\n", c);
    printf("Logical AND: %d\n", (a > 5 && b < 10));

    // 2. If-Else
    if (a > b) {
        printf("a is greater than b\n");
    } else {
        printf("b is greater or equal to a\n");
    }

    // 3. For Loop
    for (int i = 0; i < 3; i++) {
        printf("For Loop: %d\n", i);
    }

    // 4. While Loop
    int i = 0;
    while (i < 3) {
        printf("While Loop: %d\n", i);
        i++;
    }

    // 5. Do-While Loop
    i = 0;
    do {
        printf("Do-While Loop: %d\n", i);
        i++;
    } while (i < 3);

    // 6. Switch Case
    int choice = 2;
    switch (choice) {
        case 1: printf("Option 1\n"); break;
        case 2: printf("Option 2\n"); break;
        default: printf("Invalid choice\n");
    }

    // 7. Array (Integer & Char)
    int arr[3] = {1, 2, 3};
    char str[] = "Hello";
    printf("Array First Element: %d\n", arr[0]);
    printf("Character Array: %s\n", str);

    // multidimensional array
    int matrix[2][3] = { {1, 2, 3}, {4, 5, 6} };
    printf("Matrix Element [1][2]: %d\n", matrix[1][2]);

    // 8. Pointers
    int *ptr = &a;
    printf("Pointer Value: %d, Address: %p\n", *ptr, ptr);
}
//     // 9. Structure Usage
//     struct Student s1 = {"Alice", 20};
//     printf("Student Name: %s, Age: %d\n", s1.name, s1.age);

//     // 10. Printf & Scanf
//     int num;
//     printf("Enter a number: ");
//     scanf("%d", &num);
//     printf("You entered: %d\n", num);

//     // 11. Function Call with Arguments
//     greet("Alice");

//     // 12. Goto Statement
//     if (num == 1) {
//         goto label;
//     }
//     printf("This will be skipped if num == 1\n");

// label:
//     printf("Jumped to label\n");

//     // 13. Break & Continue
//     for (i = 0; i < 5; i++) {
//         if (i == 2) continue;
//         if (i == 4) break;
//         printf("Loop: %d\n", i);
//     }

//     // 14. Static Keyword
//     static int staticVar = 5;
//     printf("Static Variable: %d\n", staticVar);
    
//     return 0;
// }

// // Function Definition
// void greet(char name[]) {
//     printf("Hello, %s!\n", name);
// }
