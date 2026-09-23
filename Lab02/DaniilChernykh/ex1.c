#include<stdio.h>
#include<limits.h>
#include<float.h>

int integer = INT_MAX;
unsigned short unsigned_short = USHRT_MAX;
signed long signed_long = LONG_MAX;
float fl = FLT_MAX;
double db = DBL_MAX;

int main(void) {
    printf("TYPE\tSIZE\tVALUE\n");
    printf("INTEGER\t%lu\t%d\n", sizeof(integer), integer);
    printf("U_SHORT\t%lu\t%u\n", sizeof(unsigned_short), unsigned_short);
    printf("S_LONG\t%lu\t%ld\n", sizeof(signed_long), signed_long);
    printf("FOUAT\t%lu\t%f\n", sizeof(fl), fl);
    printf("DOUBLE\t%lu\t%f\n", sizeof(db), db);

    return 0;
}
