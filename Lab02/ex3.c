#include <stdio.h>

void convert(long long number, int source, int target)
{
    char input[64];
    char result[128];
    long long decimal = 0;
    long long power = 1;
    int length = 0;
    int i;
    int digit;

    sprintf(input, "%lld", number);

    if (source < 2 || source > 10 || target < 2 || target > 10)
    {
        printf("cannot convert!\n");
        return;
    }

    if (number == 0)
    {
        printf("0\n");
        return;
    }

    while (input[length] != '\0')
    {
        digit = input[length] - '0';

        if (digit < 0 || digit >= source)
        {
            printf("cannot convert!\n");
            return;
        }

        length++;
    }

    for (i = length - 1; i >= 0; i--)
    {
        digit = input[i] - '0';
        decimal += digit * power;
        power *= source;
    }

    length = 0;

    while (decimal > 0)
    {
        result[length] = (char)('0' + decimal % target);
        decimal /= target;
        length++;
    }

    for (i = length - 1; i >= 0; i--)
    {
        putchar(result[i]);
    }

    putchar('\n');
}

int main(void)
{
    long long number;
    int source;
    int target;

    scanf("%lld %d %d", &number, &source, &target);

    convert(number, source, target);

    return 0;
}
