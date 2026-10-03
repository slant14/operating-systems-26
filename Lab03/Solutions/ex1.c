#include<stdio.h>
#include<stdlib.h>


int const_tri(int *const p, int n) {
    int temp;
    p[0] = 0;
    p[1] = 1;
    p[2] = 1;
    if (n < 3) {
        return p[n];
    }
    while (n > 2) {
        temp = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = p[2];
        p[2] = temp;
        n--;
    }
    return p[2];
}

int main(void) {
    const int x = 1;
    const int *q = &x;
    int *const p = malloc(3 * sizeof(int));
    p[0] = *q;
    p[1] = *q;
    p[2] = 2* (*q);
    printf("Cell values: %d %d %d\n", p[0], p[1], p[2]);
    printf("Address of cell 0: %p\n", p);
    printf("Address of cell 1: %p\n", p+1);
    printf("Address of cell 2: %p\n", p+2);

    for (int i = 0; i < 10; i++) {
        printf("T(%d) = %d\n", i, const_tri(p, i));
    }
    free(p);
    return 0;
}