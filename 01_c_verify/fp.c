#include <stdio.h>

// 1. 定义几个普通的函数
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

// 2. 核心：写一个“高阶函数”，参数里接收一个函数指针
// 语法：int (*op)(int, int) 表示 op 是一个指针，指向“接收两个int，返回int”的函数
int calculate(int x, int y, int (*op)(int, int)) {
    // 这就是回调（Callback）：在这个函数里，调用了传进来的函数
    return op(x, y); 
}

int main() {
    int a = 10, b = 5;

    // 【验证一：回调函数】
    // 把 add 传进去
    printf("10 + 5 = %d\n", calculate(a, b, add));
    // 把 mul 传进去
    printf("10 * 5 = %d\n", calculate(a, b, mul));


    // 【验证二：函数指针数组】
    // 定义一个数组，里面装的全是函数指针
    int (*ops[3])(int, int) = {add, sub, mul};
    
    // 像用普通数组一样，通过下标调用
    printf("用数组调用 sub: 10 - 5 = %d\n", ops[1](a, b));

    return 0;
}
