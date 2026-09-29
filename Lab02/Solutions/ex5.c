#include <stdio.h>

int tribonacci(int n) {
    if (n == 0) {
        return 0;
    }
    int prev3 = 0;
    int prev2 = 1;
    int prev1 = 1;
    for (int i = 3; i <= n; i++) {
        int next = prev3 + prev2 + prev1;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}

int main(void) {
    printf("%d\n", tribonacci(4));
    printf("%d\n", tribonacci(36));
    return 0;
}