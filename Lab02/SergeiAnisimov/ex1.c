/* Lab 02 Sergei Anisimov CSE-04 */
#include <float.h>
#include <limits.h>
#include <stdio.h>

int main(void)
{
    int integer = INT_MAX;
    unsigned short short_integer = USHRT_MAX;
    long signed_long = LONG_MAX;
    float real_float = FLT_MAX;
    double real_double = DBL_MAX;

    printf("int: %zu bytes, max %d\n", sizeof integer, integer);
    printf("unsigned short: %zu bytes, max %hu\n", sizeof short_integer, short_integer);
    printf("signed long: %zu bytes, max %ld\n", sizeof signed_long, signed_long);
    printf("float: %zu bytes, max %.*g\n", sizeof real_float,
           FLT_DECIMAL_DIG, (double)real_float);
    printf("double: %zu bytes, max %.*g\n", sizeof real_double,
           DBL_DECIMAL_DIG, real_double);
    return 0;
}
