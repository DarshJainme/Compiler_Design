// Comprehensive Lexical Error Test Cases
// This file contains examples of all major lexical error types
#include <stdio.h>

int main() {
    
    // Type 1: Integer Overflow Errors
    int overflow1 = 2147483648;     // Exceeds INT_MAX
    int overflow2 = 9999999999999;  // Way too big
    int underflow = -2147483649;    // Exceeds INT_MIN (but this is actually two tokens: - and number)
    
    // Type 2: Illegal Characters
    @illegal_at_symbol;
    $dollar_sign_error;
    #hash_in_middle_of_code;
    `backtick_error`;
    \\backslash_error;
    
    // Type 3: Unmatched/Unterminated Literals
    char* bad_string1 = "This string is never closed;
    char* bad_string2 = "Another unclosed string
    char bad_char1 = 'x;  // Missing closing quote
    char bad_char2 = 'A;  // Another unclosed char
    
    // Type 4: Invalid Identifiers (Starting with Numbers)
    int 1variable = 10;      // Cannot start with digit
    int 2bad_name = 20;      // Another invalid start
    int 123identifier = 30;   // Starts with multiple digits
    float 9point5 = 9.5;     // Number-started identifier
    
    // Type 5: Invalid Characters in Numbers
    int bad_num1 = 123abc;     // Letters in integer
    int bad_num2 = 456def789;  // More letters
    int bad_num3 = 12$34;      // Dollar sign in number
    int bad_num4 = 67@89;      // @ symbol in number
    int bad_num5 = 45#67;      // Hash in number
    int bad_num6 = 98`12;      // Backtick in number
    float bad_float = 3.14$5;  // Invalid char in float
    
    // Type 6: Standalone Illegal Characters
    @
    $
    #
    `
    \\
    
    // Type 7: Mixed Invalid Patterns
    char array[@size];         // @ in array declaration
    int result = x $+ y;       // $ in expression
    printf("Hello")$;          // $ after statement
    
    // Valid code for comparison
    int valid_var = 42;
    char valid_char = 'A';
    char* valid_string = "This is a proper string";
    float valid_float = 3.14159;
    
    return 0;
}
