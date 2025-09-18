/* Test Case 5: Syntax Errors - Testing Error Recovery */
#include <stdio.h>

/* Missing semicolon after function declaration */
int add(int a, int b)  // ERROR: Missing semicolon

/* Function with syntax errors in parameters */
int multiply(int a, , int c) {  // ERROR: Empty parameter
    return a * c;
}

/* Missing return type */
missingReturn(int x) {  // ERROR: Missing return type
    return x * 2;
}

int main() {
    /* Unmatched braces */
    if (1 > 0) {
        printf("Test");
    // ERROR: Missing closing brace
    
    /* Invalid variable declarations */
    int ;  // ERROR: Missing variable name
    123abc = 10;  // ERROR: Invalid identifier starting with number
    int float = 5;  // ERROR: Using reserved keyword as variable name
    
    /* Missing semicolons */
    int a = 10  // ERROR: Missing semicolon
    int b = 20  // ERROR: Missing semicolon
    
    /* Unmatched parentheses */
    int result = add(a, b;  // ERROR: Missing closing parenthesis
    
    /* Invalid expressions */
    int x = 5 +;  // ERROR: Incomplete expression
    int y = * 10;  // ERROR: Invalid use of multiply operator
    
    /* Switch statement errors */
    switch (a {  // ERROR: Missing closing parenthesis
        case 1:
            printf("One");
            // ERROR: Missing break
        case 2:
            printf("Two");
            break;
        default
            printf("Default");  // ERROR: Missing colon after default
            break;
    }
    
    /* Array declaration errors */
    int array[];  // ERROR: Array size not specified
    int matrix[3][;  // ERROR: Invalid array dimensions
    
    /* Function call errors */
    printf(;  // ERROR: Missing arguments
    scanf("%d" &x);  // ERROR: Missing comma
    
    /* For loop errors */
    for (int i = 0 i < 10; i++) {  // ERROR: Missing semicolon
        printf("%d", i);
    }
    
    /* Invalid pointer operations */
    int *ptr = ;  // ERROR: Missing value
    *ptr = &5;   // ERROR: Cannot take address of literal
    
    /* Structure errors */
    struct Point {
        int x
        int y;  // ERROR: Missing semicolon after x
    }  // ERROR: Missing semicolon after structure
    
    /* Class syntax errors (if supported) */
    class Test {
        public
            int value;  // ERROR: Missing colon after public
        
        private:
            void method()  // ERROR: Missing semicolon or body
    };  // ERROR: Unmatched braces
    
    /* Goto errors */
    goto;  // ERROR: Missing label
    goto undefined_label;  // ERROR: Undefined label
    
    /* Invalid return statements */
    return;  // This might be valid depending on context
    return 0, 1;  // ERROR: Multiple return values not allowed in C
    
    /* Memory allocation errors */
    int *mem = malloc();  // ERROR: Missing size argument
    free();  // ERROR: Missing pointer argument
    
    /* Invalid type casting */
    int x = (int;  // ERROR: Incomplete cast
    float f = (float) ;  // ERROR: Missing value to cast
    
    /* Lambda expression errors (if supported) */
    auto func = lambda[] {  // ERROR: Missing parameters or body
        return 0;
    
    /* Try to use undefined variables */
    undefined_var = 100;  // ERROR: Undeclared variable
    
    /* Invalid operator usage */
    int result = 5 ++ 3;  // ERROR: Invalid operator sequence
    int another = ----;  // ERROR: Invalid operator combination
    
    /* Missing main function closing */
    // ERROR: Missing return statement and closing brace

/* ERROR: Missing closing brace for main function */

/* Function with parameter errors */
void errorFunction(int a, b) {  // ERROR: Missing type for parameter b
    printf("Error function");
}

/* Invalid preprocessor usage */
#include  // ERROR: Missing header file name
#define   // ERROR: Missing macro name

/* Invalid string and character literals */
char badChar = ';  // ERROR: Unterminated character literal
char *badString = "Unterminated string;  // ERROR: Unterminated string literal

/* Nested comment error */
/* This is a comment /* nested comment */ still in comment */ 

