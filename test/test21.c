int main() {
    int* ptr_one;
    int* ptr_two;

    // Test malloc: void* malloc(int size)
    // Allocating space for 10 integers.
    ptr_one = malloc(10 * 4); 

    // Test calloc: void* calloc(int num, int size)
    // Allocating space for 20 integers, initialized to zero.
    ptr_two = calloc(20, 4);

    return 0;
}