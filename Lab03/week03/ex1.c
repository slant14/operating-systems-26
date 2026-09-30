#include <stdio.h>
#include <stdlib.h>

int const_tri(const int *p, int n)
{
    if (n == 0) return p[0];
    if (n == 1) return p[1];
    if (n == 2) return p[2];

    int *c = (int *)p;

    for (int i = 3; i <= n; i++) {
        int temp;
        temp = c[0] + c[1] + c[2]; 
        c[0] = c[1];
        c[1] = c[2];
        c[2] = temp;
    }

    return c[2];
}

int main(void)
{
    const int x = 1;
    const int *q = &x;

    int *const p = (int *)malloc(3 * sizeof(int));

    p[0] = *q;        
    p[1] = *q;        
    p[2] = 2 * (*q);

    printf("Cell 0: %p\n", (void *)&p[0]);
    printf("Cell 1: %p\n", (void *)&p[1]);
    printf("Cell 2: %p\n", (void *)&p[2]);

    if (&p[1] - &p[0] == 1 && &p[2] - &p[1] == 1){
        printf("Contiguous.\n");
    } else {
        printf("Not contiguous.\n");
    }

    int n = 10;
    int result = const_tri(p, n);
    printf("%d", result);

    free(p);

    return 0;
}