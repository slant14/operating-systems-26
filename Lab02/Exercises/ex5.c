#include <stdio.h>

long long tribonacci(int n) {
    if (n < 0) return 0;
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;
    long long t0 = 0, t1 = 1, t2 = 1, next = 0;
    for (int i = 3; i <= n; ++i) {
        next = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    return t2;
}

int main(void) {
    printf("%lld\n", tribonacci(4));
    printf("%lld\n", tribonacci(8));
    printf("%lld\n", tribonacci(15));
    printf("%lld\n", tribonacci(28));
    printf("%lld\n", tribonacci(36));
    return 0;
}

