#include "stdio.h"
#include "limits.h"
#include "float.h"

int main() {
    int integer = INT_MAX;
    unsigned short int unsigned_short_int = USHRT_MAX;
    signed long int signed_long_int = LONG_MAX;
    float float_var = FLT_MAX;
    double double_var = DBL_MAX;

    printf("The size of int is : %zu bytes\n", sizeof(integer));
    printf("The max value of int is: %d\n", integer);
    printf("========== (^_^) ==========\n\n");
    printf("The size of unsigned short int is : %zu bytes\n", sizeof(unsigned_short_int));
    printf("The max value of unsigned short int is: %hu\n", unsigned_short_int);
    printf("========== (^_^) ==========\n\n");
    printf("The size of signed long int is : %zu bytes\n", sizeof(signed_long_int));
    printf("The max value of signed long int is: %ld\n", signed_long_int);
    printf("========== (^_^) ==========\n\n");
    printf("The size of float is : %zu bytes\n", sizeof(float_var));
    printf("The max value of float is: %f\n", float_var);
    printf("========== (^_^) ==========\n\n");
    printf("The size of double is : %zu bytes\n", sizeof(double_var));
    printf("The max value of double is: %lf\n", double_var);
    printf("========== (^_^) ==========\n\n");

    return 0;
}
