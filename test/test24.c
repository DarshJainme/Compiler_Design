// with destructor
class c{
    int x,y;
    public:
    c (int a1,int a2){
        x=a1;
        y=a2;
    }
    $c() {
        x = 0;
        y = 0;
    }
};
int main(){
    c hellow=c(123,345);
    return 0;
}