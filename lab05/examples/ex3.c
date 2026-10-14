// POSIX
// threads

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* printer(void* args) {
    pthread_t tid = pthread_self();
    for (int i = 0; i < 2; ++i) {
        printf("thread (%d) and value i (%d)\n", (long unsigned)tid, i);
        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, printer, NULL);
    pthread_create(&t2, NULL, printer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("main processes exited!\n");
}

// gcc ex3.c -pthread   