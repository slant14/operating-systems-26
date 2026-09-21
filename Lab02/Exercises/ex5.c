#include <stdio.h>

long long tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    long long t0 = 0;
    long long t1 = 1;
    long long t2 = 1;
    long long tn = 0;

    for (int i = 3; i <= n; i++) {
        tn = t0 + t1 + t2;
        t0 = t1;
        t1 = t2;
        t2 = tn;
    }
    return tn;
}

int main() {
    printf("Tribonacci(4) = %lld\n", tribonacci(4));
    printf("Tribonacci(36) = %lld\n", tribonacci(36));
    return 0;
}
