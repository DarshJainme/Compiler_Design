#include <stdio.h>

struct Point {
    int x, y;
};

class MyClass {
public:
    int value;
private:
    char* name;
protected:
    float data;
};

int main() {
    struct Point p;
    int* ptr = &p.x;
    int arr[10];
    
    static int count = 0;
    const double pi = 3.14159;
    
    printf("Hello World");
    scanf("%d", &p.x);
    
    goto label;
label:
    return 0;
}