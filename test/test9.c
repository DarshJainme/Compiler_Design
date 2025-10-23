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
    return 0;
}
