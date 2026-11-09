#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    double c = 1.5;
    const double * const p = &c;
    c += 0.5;
    // *p = c;
    // p = NULL;
    printf("%lf\n", *p);
}