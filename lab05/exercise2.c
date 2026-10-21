#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
struct Thread {
    long unsigned int id;
    int i;
    char message[256];
};

int main() {
    int n = 5;
    struct Thread threads[n];
    for (int i = 0; i < n; i++) {
    	printf("Thread %d is created\n", i);
        threads[i].i = i;
        threads[i].id = pthread_self() + i;
        snprintf(threads[i].message, 256, "Hello from thread %d",i);
        printf("Here is thread id and message: %lu, %s\n", threads[i].id, threads[i].message);
    }
}
