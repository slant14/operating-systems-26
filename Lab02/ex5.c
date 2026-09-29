#include <stdio.h>

long long tribonacci(int n)
{
    long long a = 0;
    long long b = 1;
    long long c = 1;
    long long next;

    if (n == 0)
    {
        return 0;
    }

    if (n == 1 || n == 2)
    {
        return 1;
    }

    for (int i = 3; i <= n; i++)
    {
        next = a + b + c;
        a = b;
        b = c;
        c = next;
    }

    return c;
}

int main(void)
{
    printf("%lld\n", tribonacci(4));
    printf("%lld\n", tribonacci(36));

    return 0;
}
