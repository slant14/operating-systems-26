#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int a;
    unsigned short int b;
    signed long int c;
    float d;
    double e;
    a = INT_MAX;
    b = USHRT_MAX;
    c = LONG_MAX;
    d = FLT_MAX;
    e = DBL_MAX;
    printf("%zu \n", sizeof(a));
    printf("%d\n", a);
    printf("%zu\n", sizeof(b));
    printf("%u\n", b);
    printf("%zu\n", sizeof(c));
    printf("%ld\n", c);
    printf("%zu\n", sizeof(d));
    printf("%e\n", d); 
    printf("%zu\n", sizeof(e));
    printf("%e\n", e);

    return 0;
}