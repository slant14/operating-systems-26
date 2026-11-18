#include <stdio.h>
#include <string.h>

void convert(long long x, int s, int t) {
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("cannot convert!\n");
        return;
    }
    
    //number to string
    char str[100];
    sprintf(str, "%lld", x);
    
    //check if given number is wrong
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        int digit = str[i] - '0';
        if (digit >= s) {
            printf("cannot convert!\n");
            return;
        }
    }
    
    //convert to decimal
    long long decimal = 0;
    long long power = 1;
    
    for (int i = len - 1; i >= 0; i--) {
        int digit = str[i] - '0';
        decimal += digit * power;
        power *= s;
    }
    
    //convert from decimal to target
    if (decimal == 0) {
        printf("0\n");
        return;
    }
    
    char result[100];
    int index = 0;
    
    while (decimal > 0) {
        int remainder = decimal % t;
        result[index++] = remainder + '0';
        decimal /= t;
    }
    
    //result in reverse order
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", result[i]);
    }
    printf("\n");
}

int main() {
    long long x;
    int s, t;
    scanf("%lld %d %d", &x, &s, &t);
    convert(x, s, t);
    return 0;
}
