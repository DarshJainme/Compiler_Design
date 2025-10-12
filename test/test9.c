int add(int a, int b)
{
    return a + b;
}
int main()
{
    auto add = [](int a, int b)
    {
        return a + b;
    };

    add(3, 4);
    // int (*fptr)(int, int);

    // // Assign to add()
    // fptr = &add;

    // Call the function via ptr
    // printf("%d", fptr(10, 5));
    return 0;
}
