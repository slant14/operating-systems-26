/* Lab 02 Sergei Anisimov CSE-04 */
#include <ctype.h>
#include <limits.h>
#include <stdio.h>

static long long read_number(void)
{
    int character;
    do {
        character = getchar();
    } while (character != EOF && isspace(character));

    if (character == '+')
        character = getchar();
    if (character == EOF || !isdigit(character))
        return -1;

    long long number = 0;
    do {
        int digit = character - '0';
        if (number > (LLONG_MAX - digit) / 10)
            return -1;
        number = number * 10 + digit;
        character = getchar();
    } while (character != EOF && isdigit(character));

    return character == EOF || isspace(character) ? number : -1;
}

int convert(long long number, int source, int target)
{
    if (number < 0 || source < 2 || source > 10 || target < 2 || target > 10)
        return 0;

    long long value = 0;
    long long place = 1;
    do {
        int digit = (int)(number % 10);
        if (digit >= source)
            return 0;
        value += digit * place;
        number /= 10;
        if (number > 0)
            place *= source;
    } while (number > 0);

    char result[sizeof(long long) * CHAR_BIT + 1];
    size_t index = sizeof result - 1;
    result[index] = '\0';
    do {
        result[--index] = (char)('0' + value % target);
        value /= target;
    } while (value > 0);
    puts(&result[index]);
    return 1;
}

int main(void)
{
    long long number = read_number();
    long long source = read_number();
    long long target = read_number();

    if (source < 2 || source > 10 || target < 2 || target > 10 ||
        !convert(number, (int)source, (int)target)) {
        puts("cannot convert!");
        return 1;
    }
    return 0;
}
