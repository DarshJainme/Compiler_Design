
struct node
{
    int data;
    struct node *next;
};

struct b
{
    int data;
    struct node next1;
    
};
int main(){
    struct node n1,n2;
    struct b b1;
    n1.next=&n2;
    n2.next=&n1;

}