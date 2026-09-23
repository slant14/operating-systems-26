#include<math.h>
#include<stdio.h>
#include<stdlib.h>


unsigned long long convert(unsigned long long x, int s, int t) {
    unsigned long long result_decimal = 0;
    unsigned long long result = 0;
    int power = 0;

    while (x > 0) {
        int value = x % 10;

        if (value >= s) {
            fprintf(stderr, "Error: given value has digits which are greater than given base.\n");
            exit(1);
        }

        result_decimal += value * (unsigned long long)pow((int)s, power);

        x /= 10;
        power++;
    }

    power = 0;

    while (result_decimal > 0) {
        int value = result_decimal % t;

        result += value * (unsigned long long)pow((int)10, power);

        result_decimal /= t;
        power++;
    }

    return result;
}

int main(void) {
    unsigned long long int x, result;
    int s, t;

    printf("Input your number (with base 2-10): "); // If user write something but not digits, it is their problems.
    scanf("%llu", &x);                              // Not mine. I don't care if they write 0x5BC3 right after "with
                                                    // base 2-10".
    printf("Input FROM base: ");
    scanf("%d", &s);

    printf("Input TO base: ");
    scanf("%d", &t);

    result = convert(x, s, t);
    printf("%llu converted from %d base to %d base: %llu\n", x, s, t, result);

    return 0;
}

