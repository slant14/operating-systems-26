/* Lab 05 Sergei Anisimov CSE-04 */
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Thread {
    pthread_t id;
    int i;
    char message[256];
};

static void *print_message(void *argument)
{
    struct Thread *thread = argument;
    printf("Thread ID %lu: %s\n", (unsigned long)pthread_self(), thread->message);
    return NULL;
}

int main(int argc, char *argv[])
{
    errno = 0;
    char *end = NULL;
    long count = argc == 2 ? strtol(argv[1], &end, 10) : 0;
    if (argc != 2 || errno || end == argv[1] || *end != '\0' || count < 1 || count > INT_MAX) {
        fprintf(stderr, "Usage: %s thread_count\n", argv[0]);
        return 1;
    }

    struct Thread *threads = calloc((size_t)count, sizeof *threads);
    if (threads == NULL) {
        perror("calloc");
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        struct Thread *thread = &threads[i];
        thread->i = i;
        snprintf(thread->message, sizeof thread->message, "Hello from thread %d", thread->i);
        printf("Thread %d is created\n", thread->i);
        int error = pthread_create(&thread->id, NULL, print_message, thread);
        if (error == 0)
            error = pthread_join(thread->id, NULL);
        if (error != 0) {
            fprintf(stderr, "Thread %d: %s\n", i, strerror(error));
            free(threads);
            return 1;
        }
    }
    free(threads);
    return 0;
}
