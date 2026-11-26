#include <stdio.h>
#include <stddef.h>

struct A {
    char c;
    int i;
    short s;
};

int main() {
    printf("sizeof(struct A) = %zu 字节\n",sizeof(struct A));
    printf("offset of c = %zu\n",offsetof( struct A, c));
    printf("offset of i = %zu\n",offsetof( struct A, i));
    printf("offset of s = %zu\n",offsetof( struct A, s));
    return 0;
}
