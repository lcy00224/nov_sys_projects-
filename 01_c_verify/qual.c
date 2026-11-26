#include <stdio.h>
void test_static() {
    static int s_count = 0;
    int normal_count = 0;
    s_count++;
    normal_count++;
    printf("static 计数: %d, 普通计数：%d\n",s_count,normal_count);
}

int main() {
    int a = 10, b = 20;
    const int *p1 = &a;
    int * const p2 = &a;
    const int * const p3 = &a;
    //*p1 = 100; 
    *p2 = 100;
    test_static();
    test_static();
    test_static();

    volatile int v_flag = 1;
    return 0;
}

