#include <stdio.h>
#include <stdlib.h>

enum { MIN_BASE = 2, MAX_BASE = 10 };

static void fail(void) {
    puts("cannot convert!");
    exit(EXIT_FAILURE);
}

static long long convert_to_decimal(long long x, int s) {
    long long result = 0, mult = 1;
    if (s < MIN_BASE || s > MAX_BASE || x < 0) fail();
    if (x == 0) return 0;
    while (x > 0) {
        int digit = (int)(x % 10);
        if (digit >= s) fail();
        result += mult * digit;
        mult *= s;
        x /= 10;
    }
    return result;
}

static long long convert_from_decimal(long long x, int t) {
    long long result = 0, place = 1;
    if (t < MIN_BASE || t > MAX_BASE || x < 0) fail();
    if (x == 0) return 0;
    while (x > 0) {
        int digit = (int)(x % t);
        result += place * digit;
        place *= 10;
        x /= t;
    }
    return result;
}

static long long convert(long long x, int s, int t) {
    if (s < MIN_BASE || s > MAX_BASE || t < MIN_BASE || t > MAX_BASE) fail();
    return convert_from_decimal(convert_to_decimal(x, s), t);
}

int main(void) {
    long long n;
    int s, t;
    if (scanf("%lld %d %d", &n, &s, &t) != 3) fail();
    printf("%lld\n", convert(n, s, t));
    return 0;
}

