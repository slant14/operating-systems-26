/* Lab 13 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Thread {
    pthread_t handle;
    int id;
    int waiting;
    int *requests;
    size_t count;
    bool created;
};

struct Resource {
    pthread_mutex_t mutex;
    int owner;
};

static pthread_mutex_t graph_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t graph_changed = PTHREAD_COND_INITIALIZER;
static struct Thread *threads;
static struct Resource *resources;
static int nthreads, nresources;
static bool deadlock, stop;

static int number(const char *text)
{
    errno = 0;
    char *end;
    long result = strtol(text, &end, 10);
    return errno || end == text || *end || result < 1 || result > INT_MAX ? -1 : (int)result;
}

/* A waiting thread has one outgoing request edge. A locked resource
   has one outgoing ownership edge. Follow these edges from each thread. */
static bool has_cycle(void)
{
    for (int start = 0; start < nthreads; ++start) {
        int thread = start;
        int steps = 0;
        while (thread >= 0 && threads[thread].waiting >= 0) {
            int requested = threads[thread].waiting;
            thread = resources[requested].owner;
            if (++steps == nthreads && thread >= 0 && threads[thread].waiting >= 0)
                return true;
        }
    }
    return false;
}

static void print_state(int thread)
{
    printf("Thread %d holds:", thread);
    for (int i = 0; i < nresources; ++i)
        if (resources[i].owner == thread)
            printf(" %d", i);
    if (threads[thread].waiting >= 0)
        printf(" and requests %d", threads[thread].waiting);
    putchar('\n');
}

static void *work(void *argument)
{
    struct Thread *thread = argument;
    pthread_mutex_lock(&graph_lock);
    printf("Thread %d is created\n", thread->id);
    for (size_t i = 0; i < thread->count && !stop; ++i) {
        int requested = thread->requests[i];
        struct Resource *resource = &resources[requested];
        if (resource->owner == thread->id)
            continue;
        thread->waiting = requested;
        print_state(thread->id);
        printf("Thread %d tries to lock mutex %d\n", thread->id, requested);

        while (!stop && resource->owner >= 0) {
            if (has_cycle()) {
                deadlock = stop = true;
                puts("Deadlock is detected");
                for (int id = 0; id < nthreads; ++id)
                    print_state(id);
                pthread_cond_broadcast(&graph_changed);
                break;
            }
            pthread_cond_wait(&graph_changed, &graph_lock);
        }
        thread->waiting = -1;
        if (stop)
            break;

        pthread_mutex_lock(&resource->mutex);
        resource->owner = thread->id;
        pthread_mutex_unlock(&graph_lock);

        /* Keep acquired locks while requesting the rest. This allows a cycle
           to form if threads request the same resources in opposite orders. */
        unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)thread->id ^
                            (unsigned int)i;
        struct timespec delay = {.tv_sec = 1 + rand_r(&seed) % 4};
        while (nanosleep(&delay, &delay) == -1 && errno == EINTR)
            continue;
        pthread_mutex_lock(&graph_lock);
    }

    for (int i = 0; i < nresources; ++i) {
        if (resources[i].owner == thread->id) {
            pthread_mutex_unlock(&resources[i].mutex);
            resources[i].owner = -1;
            printf("Thread %d unlocked mutex %d\n", thread->id, i);
        }
    }
    pthread_cond_broadcast(&graph_changed);
    pthread_mutex_unlock(&graph_lock);
    return NULL;
}

static int read_requests(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file) {
        perror(path);
        return 1;
    }
    char *line = NULL;
    size_t capacity = 0;
    int result = 0;
    bool header_allowed = true;
    while (getline(&line, &capacity, file) != -1) {
        char *cursor;
        char *first = strtok_r(line, " \t\r\n", &cursor);
        char *second = strtok_r(NULL, " \t\r\n", &cursor);
        if (!first)
            continue;
        if (header_allowed && second && strcmp(first, "thread") == 0 &&
            strcmp(second, "resource") == 0) {
            header_allowed = false;
            continue;
        }
        header_allowed = false;
        char *end1, *end2;
        errno = 0;
        long thread = strtol(first, &end1, 10);
        long resource = second ? strtol(second, &end2, 10) : -1;
        if (errno || *end1 || !second || *end2 ||
            strtok_r(NULL, " \t\r\n", &cursor) ||
            thread < 0 || thread >= nthreads || resource < 0 || resource >= nresources) {
            fputs("Invalid resource request.\n", stderr);
            result = 1;
            break;
        }
        struct Thread *entry = &threads[thread];
        if (entry->count == (size_t)-1 / sizeof *entry->requests) {
            result = 1;
            break;
        }
        int *list = realloc(entry->requests, (entry->count + 1) * sizeof *list);
        if (!list) {
            perror("realloc");
            result = 1;
            break;
        }
        entry->requests = list;
        entry->requests[entry->count++] = (int)resource;
    }
    result |= ferror(file) != 0;
    free(line);
    fclose(file);
    return result;
}

int main(int argc, char **argv)
{
    nresources = argc >= 3 ? number(argv[1]) : -1;
    nthreads = argc >= 3 ? number(argv[2]) : -1;
    if ((argc != 3 && argc != 4) || nresources < 1 || nthreads < 1) {
        fprintf(stderr, "Usage: %s resources threads [input.txt]\n", argv[0]);
        return 2;
    }
    threads = calloc((size_t)nthreads, sizeof *threads);
    resources = calloc((size_t)nresources, sizeof *resources);
    if (!threads || !resources) {
        perror("calloc");
        free(threads);
        free(resources);
        return 1;
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].id = i;
        threads[i].waiting = -1;
    }

    int result = read_requests(argc == 4 ? argv[3] : "input.txt");
    int initialized = 0;
    if (!result) {
        for (; initialized < nresources; ++initialized) {
            resources[initialized].owner = -1;
            int error = pthread_mutex_init(&resources[initialized].mutex, NULL);
            if (error) {
                fprintf(stderr, "pthread_mutex_init: %s\n", strerror(error));
                result = 1;
                break;
            }
        }
    }
    if (!result) {
        pthread_mutex_lock(&graph_lock);
        for (int i = 0; i < nthreads; ++i) {
            if (threads[i].count == 0)
                continue;
            int error = pthread_create(&threads[i].handle, NULL, work, &threads[i]);
            if (error) {
                fprintf(stderr, "pthread_create: %s\n", strerror(error));
                result = 1;
                stop = true;
                break;
            }
            threads[i].created = true;
        }
        pthread_mutex_unlock(&graph_lock);
    }

    for (int i = 0; i < nthreads; ++i) {
        if (threads[i].created)
            pthread_join(threads[i].handle, NULL);
        free(threads[i].requests);
    }
    if (!result && !deadlock)
        puts("No deadlocks");
    for (int i = 0; i < initialized; ++i)
        pthread_mutex_destroy(&resources[i].mutex);
    free(threads);
    free(resources);
    pthread_cond_destroy(&graph_changed);
    pthread_mutex_destroy(&graph_lock);
    return result;
}
