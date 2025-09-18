
class Animal {
public:
    int age;
    
    void speak() {
        printf("Animal speaks\n");
    }
};

class Dog : public Animal {
private:
    char breed[20];
    
public:
    void bark() {
        printf("Dog barks\n");
    }
    
    void setBreed(char *b) {
        // breed = b; // simplified
    }
};

int main() {
    Dog myDog;
    myDog.age = 3;
    myDog.speak();
    myDog.bark();
    
    Animal *pet = new Animal;
    pet->age = 5;
    pet->speak();
    delete pet;
    
    return 0;
}
