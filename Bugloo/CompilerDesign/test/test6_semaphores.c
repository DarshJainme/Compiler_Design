#include <iostream>
using namespace std;

struct Point {
    int x, y;
};

int main() {
    // Semicolon
    int a = 10;

    // Comma
    int b = 20, c = 30;

    // Parentheses
    printf("Value of a+b: %d\n", a+b);

    // Curly braces
    {
        printf("Inside curly braces\n");
    }

    // Brackets
    int arr[3] = {1, 2, 3};
    printf("First element: %d\n", arr[0]);

label:
    printf("Jumped to label\n");

    // Dot
    Point p = {5, 6};
    printf("Point: (%d, %d)\n", p.x, p.y);

    return 0;
}
