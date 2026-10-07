#include <stdio.h>

void convert(long long x, int s, int t) {
    if (s < 2 || t < 2 || s > 10 || t > 10) {
        printf("cannot convert!\n");
        return;
    }

    long long decimal = 0;
    long long power = 1;
    while (x > 0) {
        if (x % 10 >= s) {
            printf("cannot convert!\n");
            return;
        }
        decimal += (x % 10) * power;
        power *= s;
        x /= 10;
    }

    char result[65];
    int k = 0;
    do {
        result[k] = '0' + decimal % t;
        k++;
        decimal /= t;
    } while (decimal > 0);
    for (int i = k - 1; i >= 0; i--) {
        putchar(result[i]);
    }
    printf("\n");
}

int main(void) {
    long long number;
    int source, target;
    scanf("%lld %d %d", &number, &source, &target);
    convert(number, source, target);
    return 0;
}