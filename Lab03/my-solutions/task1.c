#include <stdio.h>

int main() {
    const int i = 10;
    int * p = &i;    
    *p = i;
    (*p)++;
    printf("%d\n", i);
}


// UNDEFINED BEHAVIOUR