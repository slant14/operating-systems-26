#include <stdio.h>
#include <string.h>

void convert(long long x, int s, int t) {
    if (s < 2 || s > 10 || t < 2 || t > 10) {
        printf("cannot convert!\n");
        return;
    }
    
    char str[64];
    sprintf(str, "%lld", x);
    
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        int digit;
        char single[2];
        single[0] = str[i];
        single[1] = '\0';
        
        sscanf(single, "%d", &digit);
        
        if (digit < 0 || digit >= s) {
            printf("cannot convert!\n");
            return;
        }
    }
    
    long long decimal = 0;
    long long power = 1;
    for (int i = len - 1; i >= 0; i--) {
        int digit;
        char single[2];
        single[0] = str[i];
        single[1] = '\0';
        
        sscanf(single, "%d", &digit);
        
        decimal += digit * power;
        power *= s;
    }

    char result[64];
    char temp[2];
    int index = 0;
    while (decimal > 0) {
        int remainder = decimal % t;
        
        sprintf(temp, "%d", remainder);
        result[index] = temp[0];
        index++;
        
        decimal /= t;
    }
    result[index] = '\0';
    
    for (int i = 0; i < index / 2; i++) {
        char c = result[i];
        result[i] = result[index - 1 - i];
        result[index - 1 - i] = c;
    }
    
    printf("%s\n", result);
}

int main() {
    long long number;
    int source, target;
    
    scanf("%lld %d %d", &number, &source, &target);
    
    convert(number, source, target);
    
    return 0;
}
