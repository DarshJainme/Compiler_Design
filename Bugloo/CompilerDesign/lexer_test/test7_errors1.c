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

    // Type 1: Exceeding length of numeric constants
    int overflow = 2147483648;  // Integer overflow error
    
    // Type 2: Appearance of illegal characters
    @invalidToken      // '@' is not valid in C
    #!weird$token      // '#!' in middle of code is invalid
    int x = 12$34;     // '$' in numeric constant
    $money             // '$' is invalid identifier start
    char ch = 'A@';    // Invalid character in char literal
    
    // Type 3: Identifier starting with number (spelling error)
    int 123abc = 10;   // Identifier cannot start with number
    int 3num = 1234;   // Another invalid identifier
    
    // Type 4: Invalid characters in numbers
    int num1 = 12abc34;  // Alphabetic chars in number
    int num2 = 45#67;    // Special char in number
    int num3 = 89@12;    // @ symbol in number
    
    // Type 5: More illegal characters
    char illegal_chars[] = {`backtick`, @symbol, #hash};
    
    // Type 6: Transposition and spelling errors (would be caught as invalid identifiers)
    // These are actually valid identifiers, so they won't be lexical errors
    // int mian = 5;  // This would be a semantic error, not lexical
    
    // Type 7: Mixed invalid patterns
    float bad_float = 3.14$5;  // Invalid char in float
    ^^^                        // Triple caret, not a valid operator

    return 0;
}
