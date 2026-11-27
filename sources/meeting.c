#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

extern pthread_t retrieval;

void *meeting(void *arg)
{
    (void)arg;
    void *fabric = NULL;
    if (pthread_join(retrieval, &fabric) == 0 && fabric != NULL) {
        printf("%s\n", (char *)fabric);
        fflush(stdout);
    }
    return NULL;
}
