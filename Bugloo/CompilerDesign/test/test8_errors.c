int main() {
    int valid = 10;
    
    // Illegal characters in identifiers and constants
    int bad_token@ = 5;  // Invalid character in identifier
    int another$ = 10;   // Another invalid character
    
    // Unterminated string literals
    char* unterminated = "this string never ends;
    char* another_bad = "missing quote at end;
    
    // Unterminated character literals  
    char bad_char = 'x;
    char missing_quote = 'A;
    
    // Invalid numeric constants
    int bad_number1 = 123abc;     // Number with letters
    int bad_number2 = 456def789;  // More letters in number
    int bad_number3 = 12$45;      // Dollar sign in number
    int bad_number4 = 98@76;      // @ symbol in number
    
    // Identifiers starting with numbers
    int 1invalid = 100;
    int 2bad = 200; 
    int 9identifier = 300;
    
    // Integer overflow
    int too_big = 999999999999999999999;
    
    // Illegal standalone characters
    @ # $ ` 
    
    /* This is an unterminated comment
    
    return 0;
}
