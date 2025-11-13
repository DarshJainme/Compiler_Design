class Point {
    int x, y;
    public:
    Point(int a, int b) {
        x = a;
        y = b;
    }
};

int main() {
    Point p1 = Point(10, 20);
    Point p2 = Point(30, 40);
    return 0;
}