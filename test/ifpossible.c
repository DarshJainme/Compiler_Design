
class c{
    int x,y;
    public:
    c (int a1,int a2){
        x=a1;
        y=a2;
    }
};
int main(){
    c hellow=c(123,345);
}

// int main(){
//     c hellow = c(123,345);  // Explicit constructor call (works)
    
//     c another;  // Should call default constructor (not happening)
    
// }  // Should call destructors for 'hellow' and 'another' (not happening)
// class c{
//     int x,y;
//     public:
//     c (int a1,int a2)
// };