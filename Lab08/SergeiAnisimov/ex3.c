/* Lab 08 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

enum { SECONDS = 10, BYTES = 10 * 1024 * 1024 };

int main(void)
{
    void *blocks[SECONDS] = {0};
    void *(*volatile fill)(void *, int, size_t) = memset;
    int result = 0;
    for (int i = 0; i < SECONDS; ++i) {
        blocks[i] = malloc(BYTES);
        if (!blocks[i]) {
            perror("malloc");
            result = 1;
            break;
        }
        fill(blocks[i], 0, BYTES);
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == -1) {
            perror("getrusage");
            result = 1;
            break;
        }
        printf("Second %d: allocated=%d MiB, peak RSS=%ld KiB\n",
               i + 1, (i + 1) * 10, usage.ru_maxrss);
        fflush(stdout);
        struct timespec delay = {.tv_sec = 1};
        while (nanosleep(&delay, &delay) == -1 && errno == EINTR)
            continue;
    }
    for (int i = 0; i < SECONDS; ++i)
        free(blocks[i]);
    return result;
}
