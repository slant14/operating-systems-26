#include <stdio.h>

int main(void) {
    for (int i = 1; i <= 7; i++) {
        for (int j = 0; j < i; j++) {
            putchar('*');
        }
        printf("\n");
    }

    printf("\n");

    for (int i = 1; i <= 7; i++) {
        int k;
        if (i <= 4) {
            k = i;
        } else {
            k = 8 - i;
        }
        for (int j = 1; j <= k; j++) {
            putchar('*');
        }
        printf("\n");
    }

    printf("\n");

    for (int i = 1; i <= 7; i++) {
        for (int j = 0; j < 7; j++) {
            putchar('*');
        }
        printf("\n");
    }
    return 0;
}