#include <iostream>
using namespace std;

struct Point {
    int x, y;
    void display() { printf("Point: (%d, %d)\n", x, y); }
};

int main() {
    int a1 = 5, a=5, b = 10;

    // ++ and --
    a++;
    b--;

    // +=, -=, *=, /=, %=
    a+=2;
    b-=3;
    a*=4;
    b/=2;
    a%=5;
    // ==, !=, <=, >=
    if(a==b) {
        printf("Equal\n");
    }
    if (a!=b) {
        printf("Not Equal\n");
    }
    if (a<=b) {
        printf("a is less or equal\n");
    }
    if (a>=b) {
        printf("a is greater or equal\n");
    }

    // && and ||
    if (a > 0 && b > 0) {
        printf("Both positive\n");
    }
    if (a > 0 || b > 0) {
        printf("At least one positive\n");
    }

    // << and >>
    printf("Output operator <<\n");
    int shiftVal = 1;
    shiftVal = shiftVal << 2; // left shift
    shiftVal = shiftVal >> 1; // right shift
    printf("ShiftVal: %d\n", shiftVal);

    // -> operator
    Point p = {3, 4};
    Point* ptr = &p;
    ptr->display();

    return 0;
}