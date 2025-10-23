// Simple class definition and usage
typedef int myint;

// Global variable to demonstrate scope resolution
int age = 100;

class Animal {
public:
    int age;
    static int count;   // static member
    void speak();
private:
    int secret;
};

// Initialize static member using scope resolution
int Animal::count = 0;

class Dog : public Animal {
public:
    void bark();
};

// Define functions outside class using scope resolution
void Animal::speak() {
    // (example function body)
}

void Dog::bark() {
    // (example function body)
}

int main() {
    Animal a;
    Dog d;
    a.age = 5;

    d.age = 3;
    d.bark();
    d.speak();

    // Scope resolution operator
    int age = 10; // Local variable shadows global one
    int totalAnimals = Animal::count; // Access static variable using scope resolution

    return 0;
}