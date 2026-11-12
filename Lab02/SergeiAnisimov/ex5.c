/* Lab 02 Sergei Anisimov CSE-04 */
#include <stdio.h>

long tribonacci(int n)
{
    if (n < 2)
        return n;

    long first = 0;
    long second = 1;
    long third = 1;
    for (int index = 3; index <= n; ++index) {
        long next = first + second + third;
        first = second;
        second = third;
        third = next;
    }
    return third;
}

int main(void)
{
    printf("%ld\n%ld\n", tribonacci(4), tribonacci(36));
    return 0;
}
