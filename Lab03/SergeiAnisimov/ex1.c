/* Lab 03 Sergei Anisimov CSE-04 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int const_tri(int *const p, int n)
{
    if (n == 0)
        return 0;
    if (n <= 3)
        return p[n - 1];

    while (n-- > 3) {
        int temp = p[0] + p[1] + p[2];
        p[0] = p[1];
        p[1] = p[2];
        p[2] = temp;
    }
    return p[2];
}

int main(void)
{
    const int x = 1;
    const int *q = &x;
    int *const p = malloc(3 * sizeof *p);
    if (p == NULL) {
        perror("malloc");
        return 1;
    }

    p[0] = *q;
    p[1] = *q;
    p[2] = 2 * *q;
    printf("Addresses: %p %p %p\n", (void *)p, (void *)(p + 1), (void *)(p + 2));
    printf("Contiguous: %s\n",
           (uintptr_t)(p + 1) - (uintptr_t)p == sizeof *p &&
           (uintptr_t)(p + 2) - (uintptr_t)(p + 1) == sizeof *p ? "yes" : "no");
    printf("T(10) = %d\n", const_tri(p, 10));

    free(p);
    return 0;
}
