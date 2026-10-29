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
    printf("size of a: %ld, value of a: %d\n", sizeof(a), a);
    printf("size of b: %ld, value of b: %hu\n", sizeof(b), b);
    printf("size of c: %ld, value of c: %ld\n", sizeof(c), c);
    printf("size of d: %ld, value of d: %f\n", sizeof(d), d);
    printf("size of e: %ld, value of e: %f", sizeof(e), e);
    return 0;
}