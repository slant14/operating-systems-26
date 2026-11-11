#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// monitor memory usage of the program
#include <sys/resource.h>

int main()
{
    printf("%d\n", getpid());

    char *msg = malloc(100 * sizeof(char));
    strcpy(msg, "eeeeh what am i doing....?\n");

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("max RSS: %ld KB\n", usage.ru_maxrss);

    while (1) sleep(1);
    free(msg);
}