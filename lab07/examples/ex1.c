#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *a = malloc(5 * sizeof(int));
    if (a == NULL) return 1;

    for (int i = 0; i < 5; ++i) {
        a[i] = i * i;
    }
    for (int i = 0; i < 5; ++i) {
        printf("%d", a[i]);
    }

    // int *b = realloc(a, sizeof(int) * 10000000)
    a = realloc(a, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        printf("%d", a[i]);
    }

    free(a);
}