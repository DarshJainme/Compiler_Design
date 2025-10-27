// Function Call with Variable Arguments
int sum(int count, ...) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total++;
    }
}

void test_variadic_function() {
    int total = sum(3, 10, 20, 30);  // Expected: 60
}