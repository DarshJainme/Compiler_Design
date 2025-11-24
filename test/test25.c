
// Recursion: Factorial function
int factorial(int n) {
    if (n <= 1)
    { 
        return 1;
    }
    return n * factorial(n - 1);
}

int test_recursion() {
    int result = factorial(5);  // Expected: 120
    return result;
}

int main(){
    int c=test_recursion();
    printf("%d",c);
}