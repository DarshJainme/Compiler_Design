
struct Student {
    char name[50];
    int age;
    float grade;
};

int main() {
    struct Student s1;
    struct Student *ptr;
    
    s1.age = 20;
    s1.grade = 85.5;
    
    ptr = &s1;
    
    printf("Student age: %d\n", ptr->age);
    printf("Student grade: %.1f\n", ptr->grade);
    
    int *numbers;
    numbers = malloc(sizeof(int) * 5);
    
    int i;
    for (i = 0; i < 5; i++) {
        numbers[i] = i * 2;
    }
    
    free(numbers);
    return 0;
}
