/* Lab 05 Sergei Anisimov CSE-04 */
#include "primes.h"

static pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
static int next;
static int count;
static int limit;

static int get_number_to_check(void)
{
    int result = next;
    if (next < limit)
        ++next;
    return result;
}

static void increment_primes(void)
{
    ++count;
}

static void *check_primes(void *argument)
{
    (void)argument;
    for (;;) {
        pthread_mutex_lock(&global_lock);
        int candidate = get_number_to_check();
        pthread_mutex_unlock(&global_lock);
        if (candidate == limit)
            return NULL;
        if (is_prime(candidate)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
}

int main(int argc, char **argv)
{
    int n = argc == 3 ? parse_count(argv[1], 0) : -1;
    int m = argc == 3 ? parse_count(argv[2], 1) : -1;
    if (n < 0 || m < 1) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return 2;
    }
    limit = n;
    int result = run_threads(m, check_primes, NULL, 0);
    if (result == 0)
        printf("%d\n", count);
    pthread_mutex_destroy(&global_lock);
    return result;
}
