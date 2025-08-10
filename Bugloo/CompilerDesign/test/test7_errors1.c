#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int b = 20;

    // Valid punctuations
    printf(a+b, "\n");
    { printf("Inside curly braces\n"); }
    int arr[3] = {1, 2, 3};
    printf("First element: %d\n", arr[0]);
label:
    printf("Jumped to label\n");

    // Invalid tokens to cause lexical errors
    @invalidToken      // '@' is not valid in C++
    #!weird$token      // '#!' in middle of code is invalid
    123abc             // starts with number but has letters
    $money             // '$' is invalid identifier start
    ^^^                // triple caret, not a valid operator

    return 0;
}
