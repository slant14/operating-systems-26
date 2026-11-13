/* Lab 05 Sergei Anisimov CSE-04 */
#ifndef PRIMES_H
#define PRIMES_H

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_count(const char *text, int minimum)
{
    errno = 0;
    char *end;
    long value = strtol(text, &end, 10);
    return errno || end == text || *end || value < minimum || value > INT_MAX
           ? -1 : (int)value;
}

static bool is_prime(int number)
{
    if (number < 2)
        return false;
    if (number % 2 == 0)
        return number == 2;
    for (int divisor = 3; divisor <= number / divisor; divisor += 2)
        if (number % divisor == 0)
            return false;
    return true;
}

static int run_threads(int count, void *(*work)(void *), void *data, size_t stride)
{
    pthread_t *threads = calloc((size_t)count, sizeof *threads);
    if (!threads) {
        perror("calloc");
        return 1;
    }
    int created = 0;
    int error = 0;
    for (; created < count; ++created) {
        void *arg = data ? (char *)data + (size_t)created * stride : NULL;
        error = pthread_create(&threads[created], NULL, work, arg);
        if (error)
            break;
    }
    for (int i = 0; i < created; ++i) {
        int joined = pthread_join(threads[i], NULL);
        if (joined && !error)
            error = joined;
    }
    free(threads);
    if (error)
        fprintf(stderr, "pthread: %s\n", strerror(error));
    return error != 0;
}

#endif
