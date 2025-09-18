
/* Test Case: Comprehensive Correct Syntax */
#include <stdio.h>
#include <stdlib.h>

/* Global variables */
int global_var = 10;
static int static_var;
char global_char = 'A';
float global_float = 3.14;

/* Structure declaration */
struct Point {
    int x;
    int y;
    char label;
};

/* Function declarations */
int add(int a, int b);
void print_array(int arr[], int size);
float calculate_average(int numbers[], int count);

/* Function definitions */
int add(int a, int b) {
    return a + b;
}

void print_array(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
}

float calculate_average(int numbers[], int count) {
    int sum = 0;
    int i;
    
    for (i = 0; i < count; i++) {
        sum += numbers[i];
    }
    
    return (float)sum / count;
}

/* Main function */
int main() {
    /* Local variable declarations */
    int x = 5;
    int y = 10;
    int result;
    char ch = 'B';
    float avg;
    
    /* Arrays */
    int numbers[5] = {1, 2, 3, 4, 5};
    char string[20] = "Hello World";
    
    /* Pointers */
    int *ptr = &x;
    char *str_ptr = string;
    
    /* Structure variables */
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    p1.label = 'P';
    
    /* Arithmetic operations */
    result = add(x, y);
    result = x + y * 2 - 1;
    result = (x + y) / 2;
    
    /* Comparison operations */
    if (x > y) {
        printf("x is greater than y\n");
    } else if (x < y) {
        printf("x is less than y\n");
    } else {
        printf("x equals y\n");
    }
    
    /* Loops */
    int i;
    for (i = 0; i < 5; i++) {
        printf("Loop iteration: %d\n", i);
    }
    
    i = 0;
    while (i < 3) {
        printf("While loop: %d\n", i);
        i++;
    }
    
    i = 0;
    do {
        printf("Do-while loop: %d\n", i);
        i++;
    } while (i < 2);
    
    /* Switch statement */
    switch (x) {
        case 1:
            printf("x is 1\n");
            break;
        case 5:
            printf("x is 5\n");
            break;
        default:
            printf("x is something else\n");
            break;
    }
    
    /* Function calls */
    print_array(numbers, 5);
    avg = calculate_average(numbers, 5);
    printf("Average: %.2f\n", avg);
    
    /* Pointer operations */
    printf("Value of x: %d\n", *ptr);
    printf("Address of x: %p\n", (void*)ptr);
    
    /* Goto and labels */
    if (x > 0) {
        goto positive_label;
    }
    
    printf("This won't be printed\n");
    
positive_label:
    printf("x is positive\n");
    
    return 0;
}
