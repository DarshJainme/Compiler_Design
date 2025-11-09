// Simple class definition and usage
typedef int myint;

// Global variable to demonstrate scope resolution
int age = 100;

class Animal {
public:
    int age;
    static int count;   // static member

    // Constructor and Destructor prototypes
    Animal();
    $Animal();

    void speak();
private:
    int secret;
};

// Initialize static member using scope resolution
int Animal::count = 0;

// Define Animal constructor
Animal::Animal() {
    // Constructor body
    count++; // Example: Increment count when an Animal is created
}

// Define Animal destructor
Animal::$Animal() {
    // Destructor body
}

class Dog : public Animal {
public:
    // Constructor and Destructor prototypes
    Dog();
    $Dog();

    void bark();
};

// Define Dog constructor
Dog::Dog() {
    // Dog constructor body
}

// Define Dog destructor
Dog::$Dog() {
    // Dog destructor body
}

// Define functions outside class using scope resolution
void Animal::speak() {
    // (example function body)
}

void Dog::bark() {
    // (example function body)
}

class c{
    int x,y;
    public:
    c (int a1,int a2)
};
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