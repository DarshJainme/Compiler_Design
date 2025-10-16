// testing the double pointers

int main() {
    // Step 1: Create a normal integer variable
    int a = 10;

    // Step 2: Create a single pointer to 'a'
    int *ptr = &a;

    // Step 3: Create a double pointer to 'ptr'
    int **dptr = &ptr;

    // Step 4: Accessing values
    // printf("Value of a: %d\n", a);
    // printf("Value of a using *ptr: %d\n", *ptr);
    // printf("Value of a using **dptr: %d\n", **dptr);

    // Step 5: Changing the value using double pointer
    **dptr = 25;
    // printf("New value of a after changing via **dptr: %d\n", a);

    return 0;
}
