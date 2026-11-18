#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int integer;
    unsigned short int ushort;
    signed long int slong;
    float float_v;
    double double_v;
    
    integer = INT_MAX;
    ushort = USHRT_MAX;
    slong = LONG_MAX;
    float_v = FLT_MAX;
    double_v = DBL_MAX;
    
    printf("Integer:\n");
    printf("Size: %zu bytes\n", sizeof(integer));
    printf("Maximum value: %d\n\n", integer);
    
    printf("Unsigned Short Integer:\n");
    printf("Size: %zu bytes\n", sizeof(ushort));
    printf("Maximum value: %u\n\n", ushort);
    
    printf("Signed Long Integer:\n");
    printf("Size: %zu bytes\n", sizeof(slong));
    printf("Maximum value: %ld\n\n", slong);
    
    printf("Float:\n");
    printf("Size: %zu bytes\n", sizeof(float_v));
    printf("Maximum value: %e\n\n", float_v);
    
    printf("Double:\n");
    printf("Size: %zu bytes\n", sizeof(double_v));
    printf("Maximum value: %e\n", double_v);
    
    return 0;
}
