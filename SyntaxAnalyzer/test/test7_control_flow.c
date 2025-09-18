
int main() {
    int choice = 2;
    int i = 0;
    
    switch (choice) {
        case 1:
            printf("Choice is 1\n");
            break;
        case 2:
            printf("Choice is 2\n");
            break;
        default:
            printf("Unknown choice\n");
            break;
    }
    
    while (i < 5) {
        if (i == 3) {
            i++;
            continue;
        }
        printf("i = %d\n", i);
        i++;
    }
    
    do {
        printf("Do-while loop\n");
        i--;
    } while (i > 3);
    
    for (i = 0; i < 3; i++) {
        if (i == 2) {
            break;
        }
        printf("For loop: %d\n", i);
    }
    
    goto end;
    printf("This won't print\n");
    
end:
    printf("End of program\n");
    return 0;
}
