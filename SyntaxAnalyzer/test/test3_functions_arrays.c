
/* Test file for functions and arrays */

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void array_operations() {
    int arr[10];
    int i;
    
    // Initialize array
    for (i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }
    
    // Print array
    for (i = 0; i < 10; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
}

void string_operations() {
    char str[100];
    char str2[100];
    int len;
    
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = '\0';
    
    len = 0;
    while (str[len] != '\0') {
        len++;
    }
    
    printf("String length: %d\n", len);
}

int main() {
    int result;
    
    // Test factorial
    result = factorial(5);
    printf("Factorial of 5: %d\n", result);
    
    // Test fibonacci (limited to prevent infinite recursion)
    result = fibonacci(10);
    printf("Fibonacci of 10: %d\n", result);
    
    // Test arrays
    array_operations();
    
    // Test strings
    string_operations();
    
    return 0;
}
