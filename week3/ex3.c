#include <stdio.h>

void convert(long long x, int s, int t) {
    if (s > 10 || s < 2 || t > 10 || t < 2) {
        printf("cannot convert!");
        return;
    }
    long long tmp = x;
    int digit;
    while (tmp != 0) {
        digit = tmp % 10;
        tmp /= 10;
        if (digit >= s) {
            printf("cannot convert!");
            return;
        }
    }
    long long decimalValue = 0;
    long long multiplier = 1;
    tmp = x;
    while (tmp != 0) {
        digit = tmp % 10;
        decimalValue += digit * multiplier;
        multiplier *= s;
        tmp /= 10;
    }
    char convertedReversed[1000];
    int curLength = 0;
    if (decimalValue == 0) {
        convertedReversed[curLength] = '0';
        curLength++;
    }
    while (decimalValue != 0) {
        digit = decimalValue % t;
        convertedReversed[curLength] = digit + '0';
        decimalValue /= t;
        curLength++;
    }
    curLength--;
    for (int i = curLength; i >= 0; i--) {
        printf("%c", convertedReversed[i]);
    }
}

int main() {
    long long x;
    int s;
    int t;
    scanf("%lld", &x);
    scanf("%d", &s);
    scanf("%d", &t);
    convert(x, s, t);
    return 0;
}