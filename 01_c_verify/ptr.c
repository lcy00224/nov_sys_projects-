#include <stdio.h>

int main() {
    int x = 42;
    int *p = &x;      
    int **pp = &p;    

    printf("x 的值: %d\n", x);
    
    **pp = 100; 
    
    printf("修改后 x 的值: %d\n", x);
    
    printf("x 的地址: %p\n", (void*)&x);
    printf("p 的值: %p\n", (void*)p);
    printf("pp 的值: %p\n", (void*)pp);

    return 0;
}
