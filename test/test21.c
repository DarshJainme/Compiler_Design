int main() {
    int *ptr_one;
    int *ptr_two;

    ptr_one = malloc(10 * sizeof *ptr_one);
    if (!ptr_one) return 1;

    ptr_two = calloc(20, sizeof *ptr_two);
    if (!ptr_two) { return 1; }

    /* initialize ptr_one */
    // ptr_one[1] = 100;
    int i1=0;
    i1++;
    // for (int i = 0; i < 10; ++i) {
    //     ptr_one[i]=i;
    // }

    // ptr_two[i1] = 1009;
    // /* initialize ptr_two */
    // for (int i = 0; i < 20; ++i) {
    //     ptr_two[i]=i+100;
    // }

    // printf("%d\n", ptr_one[0]);
    // printf("%d", ptr_two[i1]);
    return 0;
}