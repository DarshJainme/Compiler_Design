// Simple class definition and usage
typedef int myint;

class Animal {
public:
    int age;
    void speak();
private:
    int secret;
};

class Dog : public Animal {
public:
    void bark();
};

int main() {
    Animal a;
    Dog d;
    a.age = 5;
    d.age = 3;
    d.bark();
    d.speak();
    return 0;
}
