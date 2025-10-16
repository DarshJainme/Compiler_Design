// Simple class definition and usage
typedef int myint;

// Forward declare Animal so getSecret can use it
class Animal; 

// Declare a global function that will be a friend
void setSecret(Animal &a, int s);

class Animal {
public:
    int age;
    void speak();
    // Declare a friend function
    friend void setSecret(Animal& a, int s);
private:
    int secret;
};

// Implement the friend function
void setSecret(Animal& a, int s) {
    a.secret = s; // OK: friend function can access private members
}

class Dog : public Animal {
public:
    void bark();
};

int main() {
    Animal a;
    Dog d;
    a.age = 5;
    // a.secret = 10; // ERROR: main cannot access private member
    setSecret(a, 10); // OK: friend function sets the secret

    d.age = 3;
    d.bark();
    d.speak();
    return 0;
}