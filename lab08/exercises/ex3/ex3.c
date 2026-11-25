#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

#define TEN_MB (10 * 1024 * 1024)

int main()
{
    struct rusage usage;

    for (int i = 0; i < 10; i++) {
        char *ptr = malloc(TEN_MB);
        memset(ptr, 0, TEN_MB);

        getrusage(RUSAGE_SELF, &usage);
        printf("max RSS: %ld KB\n", usage.ru_maxrss);

        sleep(1);
    }
}