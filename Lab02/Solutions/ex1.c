#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void) {
    int i = INT_MAX;
    unsigned short int u = USHRT_MAX;
    signed long int s = LONG_MAX;
    float f = FLT_MAX;
    double d = DBL_MAX;
    printf("Integer: size = %zu, maximum value = %d\n", sizeof(i), i);
    printf("Unsigned short integer: size = %zu, maximum value = %hu\n", sizeof(u), u);
    printf("Signed long int: size = %zu, maximum value = %ld\n", sizeof(s), s);
    printf("Float: size = %zu, maximum value = %f\n", sizeof(f), f);
    printf("Double: size = %zu, maximum value = %f\n", sizeof(d), d);
    return 0;
}