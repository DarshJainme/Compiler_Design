// Simple class definition and usage
typedef int myint;

// Forward declare Animal so getSecret can use it
class Animal; 

// Declare a global function that will be a friend
void setSecret(Animal &a, int s);

// Global variable to demonstrate scope resolution
int age = 100;

class Animal {
public:
    int age;
    static int count;   // static member
    void speak();
    // Declare a friend function
    friend void setSecret(Animal& a, int s);
private:
    int secret;
};

// Initialize static member using scope resolution
int Animal::count = 0;

// Implement the friend function
void setSecret(Animal& a, int s) {
    a.secret = s; // OK: friend function can access private members
}

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
    // a.secret = 10; // ERROR: main cannot access private member
    setSecret(a, 10); // OK: friend function sets the secret

    d.age = 3;
    d.bark();
    d.speak();

    // Scope resolution operator
    int age = 10; // Local variable shadows global one
    int totalAnimals = Animal::count; // Access static variable using scope resolution

    return 0;
}