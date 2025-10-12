// Arithmetic and logical operators (without comparison operators)
int arithmetic_logical() {
    int a = 10, b = 5, c;
    c = a + b - 3 * 2 / 1;   // Arithmetic only
    c = a && b;              // Logical AND
    c = a | b;               // Bitwise
    return c;
}

// if-else (using only equality)
void test_if_else(int x) {
    if (x == 0)
        x = 1;
    else
        x = -1;
}

// for loop (using assignment and increment only)
void test_for_loop() {
    int i;
    i = 0;
    while (i != 5) {
        int x = i * 2;
        i++;
    }
}

// while loop (using not equal)
void test_while_loop() {
    int i = 0;
    while (i != 5) {
        i++;
    }
}

// do-while loop (using not equal)
void test_do_while() {
    int i = 0;
    do {
        i++;
    } while (i != 5);
}

// switch case
void test_switch(int x) {
    switch (x) {
        case 1: x = 10; break;
        case 2: x = 20; break;
        default: x = 0;
    }
}

// Arrays (integer and char)
void test_arrays() {
    int arr[5];
    char str[10];
    arr[2] = 10;
}

// Pointers
void test_pointers() {
    int a = 10;
    int *p = &a;
    *p = 20;
}

// Structure
struct TestStruct {
    int a;
    char b;
};

void test_structure() {
    struct TestStruct s;
    s.a = 10;
    s.b = 'X';
}

// Function call with arguments
int add(int a, int b) {
    return a + b;
}

void test_function_call() {
    int x = add(3, 4);
}

// goto
void test_goto() {
    int x = 0;
    goto label;
    x = 1;
label:
    x = 2;
}

// break and continue
void test_break_continue() {
    int i;
    i = 0;
    while (i != 10) {
        if (i == 5) {
            i++;
            continue;
        }
        if (i == 8) break;
        i++;
    }
}

// Static keyword
void test_static() {
    static int count = 0;
    count++;
}