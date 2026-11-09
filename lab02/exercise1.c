#include <stdio.h>
#include <limits.h>
#include <float.h>
int main() {
    int a;
    unsigned short int b;
    float c;
    double d;

    a = INT_MAX;
    b = USHRT_MAX;
    c = FLT_MAX;
    d = DBL_MAX;

    printf("%zu\n%d\n",sizeof(a),a);
    printf("%zu\n%d\n",sizeof(b),b);
    printf("%zu\n%f\n",sizeof(c),c);
    printf("%zu\n%f\n",sizeof(d),d);
}
