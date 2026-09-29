#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    int integerValue = INT_MAX;
    unsigned short unsignedShortValue = USHRT_MAX;
    signed long int longValue = LONG_MAX;
    float floatValue = FLT_MAX;
    double doubleValue = DBL_MAX;

    printf("int: size = %zu, max = %d\n",
           sizeof(integerValue), integerValue);

    printf("unsigned short int: size = %zu, max = %hu\n",
           sizeof(unsignedShortValue), unsignedShortValue);

    printf("signed long int: size = %zu, max = %ld\n",
           sizeof(longValue), longValue);

    printf("float: size = %zu, max = %e\n",
           sizeof(floatValue), floatValue);

    printf("double: size = %zu, max = %e\n",
           sizeof(doubleValue), doubleValue);

    return 0;
}
