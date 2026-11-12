#include <stdio.h>

int main() {
    int n;
    int firstTmp = 0;
    int secondTmp = 1;
    int tmp = 0;
    int res = 0;
    scanf("%d", &n);
    if (n == 0) {
        res = firstTmp;
    }
    if (n == 1) {
        res = secondTmp;
    }
    if (n > 1) {
        for (int i = 1; i < n; i++) {
            tmp = firstTmp + secondTmp;
            firstTmp = secondTmp;
            secondTmp = tmp;
        }
        res = secondTmp;
    }
    printf("%d", res);
    printf("\n");
    return 0;
}