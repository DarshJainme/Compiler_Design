
struct Point {
    int x;
    int y;
};

class Rectangle {
public:
    int width;
    int height;
    
    int area() {
        return width * height;
    }
};

int main() {
    struct Point p;
    p.x = 10;
    p.y = 20;
    
    int *ptr;
    ptr = malloc(sizeof(int) * 5);
    
    Rectangle rect;
    rect.width = 5;
    rect.height = 10;
    
    printf("Point: (%d, %d)\n", p.x, p.y);
    printf("Rectangle area: %d\n", rect.area());
    
    free(ptr);
    return 0;
}
