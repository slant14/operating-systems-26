/* Lab 05 Sergei Anisimov CSE-04 */
#include "primes.h"

struct Request {
    int first;
    int end;
    int primes;
};

static void *count_primes(void *argument)
{
    struct Request *request = argument;
    for (int i = request->first; i < request->end; ++i)
        request->primes += is_prime(i);
    return NULL;
}

int main(int argc, char **argv)
{
    int n = argc == 3 ? parse_count(argv[1], 0) : -1;
    int m = argc == 3 ? parse_count(argv[2], 1) : -1;
    if (n < 0 || m < 1) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 2;
    }
    struct Request *requests = calloc((size_t)m, sizeof *requests);
    if (!requests) {
        perror("calloc");
        return 1;
    }
    for (int i = 0; i < m; ++i) {
        requests[i].first = (int)((long long)n * i / m);
        requests[i].end = (int)((long long)n * (i + 1) / m);
    }
    int result = run_threads(m, count_primes, requests, sizeof *requests);
    if (result == 0) {
        int total = 0;
        for (int i = 0; i < m; ++i)
            total += requests[i].primes;
        printf("%d\n", total);
    }
    free(requests);
    return result;
}
