/* Lab 03 Sergei Anisimov CSE-04 */
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void *aggregate(void *base, size_t size, int n, void *initial_value,
                void *(*opr)(const void *, const void *))
{
    unsigned char *element = base;
    for (int i = 0; i < n; ++i, element += size)
        memmove(initial_value, opr(initial_value, element), size);

    return initial_value;
}

/* The variable arguments are const void pointers to values of size bytes. */
void *vaggregate(size_t size, int n, void *initial_value,
                 void *(*opr)(const void *, const void *), ...)
{
    va_list args;
    va_start(args, opr);
    for (int i = 0; i < n; ++i) {
        const void *element = va_arg(args, const void *);
        memmove(initial_value, opr(initial_value, element), size);
    }
    va_end(args);
    return initial_value;
}

static void *add_int(const void *a, const void *b)
{
    static int result;
    result = *(const int *)a + *(const int *)b;
    return &result;
}

static void *multiply_int(const void *a, const void *b)
{
    static int result;
    result = *(const int *)a * *(const int *)b;
    return &result;
}

static void *max_int(const void *a, const void *b)
{
    static int result;
    int left = *(const int *)a;
    int right = *(const int *)b;
    result = left > right ? left : right;
    return &result;
}

static void *add_double(const void *a, const void *b)
{
    static double result;
    result = *(const double *)a + *(const double *)b;
    return &result;
}

static void *multiply_double(const void *a, const void *b)
{
    static double result;
    result = *(const double *)a * *(const double *)b;
    return &result;
}

static void *max_double(const void *a, const void *b)
{
    static double result;
    double left = *(const double *)a;
    double right = *(const double *)b;
    result = left > right ? left : right;
    return &result;
}

int main(void)
{
    int integers[] = {1, 2, 3, 4, 5};
    double doubles[] = {-1.5, -2, -0.5, -4, -3};
    int int_sum = 0, int_product = 1, int_max = INT_MIN;
    double sum = 0, product = 1, max = -DBL_MAX;

    aggregate(integers, sizeof *integers, 5, &int_sum, add_int);
    aggregate(integers, sizeof *integers, 5, &int_product, multiply_int);
    aggregate(integers, sizeof *integers, 5, &int_max, max_int);
    aggregate(doubles, sizeof *doubles, 5, &sum, add_double);
    aggregate(doubles, sizeof *doubles, 5, &product, multiply_double);
    aggregate(doubles, sizeof *doubles, 5, &max, max_double);

    printf("Integers: sum=%d product=%d max=%d\n", int_sum, int_product, int_max);
    printf("Doubles: sum=%g product=%g max=%g\n", sum, product, max);

    int_sum = 0;
    vaggregate(sizeof *integers, 5, &int_sum, add_int,
               (const void *)&integers[0], (const void *)&integers[1],
               (const void *)&integers[2], (const void *)&integers[3],
               (const void *)&integers[4]);
    printf("Variadic integer sum: %d\n", int_sum);
    return 0;
}
