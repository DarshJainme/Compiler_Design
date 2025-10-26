int main()
{
    int a=0,b=0;
    b=a++;
    b=++a;
    b=a--;
    b=--a;
    int t0000=b;
    int t1111=b;
    t0000=t1111++;
    t1111=++t0000;
    return 0;
}