#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(){
	int a = INT_MAX;
	printf("int: %zu bytes, %d value\n", sizeof(a), a);
	unsigned int b = USHRT_MAX;
	printf("int: %zu bytes, %u value\n", sizeof(b), b);
	long int c = LONG_MAX;
	printf("int: %zu bytes, %ld value\n", sizeof(c), c);
	float d = FLT_MAX;
	printf("int: %zu bytes, %e value\n", sizeof(d), d);
	double e = DBL_MAX;
	printf("int: %zu bytes, %e value\n", sizeof(e), e);
}
