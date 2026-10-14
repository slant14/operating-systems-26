// POSIX
// threads

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
void* printer(void* args) {
    for (int i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&m);
        ++counter;
        pthread_mutex_unlock(&m);
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

    printf("expecting: %d\n", 2000000);
    printf("getting: %d\n", counter);

    pthread_mutex_destroy(&m);
}

// gcc ex3.c -pthread   

// expecting: 2000000
// getting: 1333003
// 2 threads are overwriting each other
// solution: 1) put the lock while one thread works -> mutex