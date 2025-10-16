// C Parser Advanced Features Test Cases

// Recursion: Factorial function
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

void test_recursion() {
    int result = factorial(5);  // Expected: 120
}

// // Function Call with Variable Arguments
// int sum(int count, ...) {
//     int total = 0;
//     for (int i = 0; i < count; i++) {
//         auto k =  args[i];
//     }
// }

// void test_variadic_function() {
//     int total = sum(3, 10, 20, 30);  // Expected: 60
// }

// Dynamic Memory Allocation
// void test_dynamic_memory() {
//     int *arr = (int *)malloc(5 * sizeof(int));  // Allocating memory
//     if (!arr) return;
    
//     for (int i = 0; i < 5; i++)
//         arr[i] = i * 10;  // Storing values
    
//     free(arr);  // Freeing memory
// }

// Command Line Input
// int main(int argc, char *argv[]) {
//     if (argc > 1) {
//         int number = atoi(argv[1]);  // Convert argument to integer
//     }
//     return 0;
// }

// Enum and Union
enum Color { RED, GREEN, BLUE };  // Enum example

union Data {
    int i;
    float f;
};  // Union example

void test_enum_union() {
    enum Color c = GREEN;
    union Data d;
    d.i = 10;  // Using union
}

// File Manipulation
// void test_file_operations() {
//     FILE *fp = fopen("test.txt", "w");  // Opening file for writing
//     if (!fp) return;

//     fprintf(fp, "Hello, World!");  // Writing to file
//     fclose(fp);  // Closing file
// }

// "Until" Loop (Simulated using do-while)
void test_until_loop() {
    int x = 0;
    do {
        x++;
    } while (x < 5);  // Equivalent to "until (x >= 5)"
}

// Multi-Level Pointers
void test_multilevel_pointers() {
    int a = 10;
    int *p = &a;
    // int **pp = &p;
    // int ***ppp = &pp;  // Triple pointer
}

// Multi-Dimensional Arrays
void test_multidimensional_arrays() {
    int arr[2][3] = { {1, 2, 3}, {4, 5, 6} };
    int x = arr[1][2];  // Accessing element (Expected: 6)
}
