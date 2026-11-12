/* Lab 04 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum { MAX_ROUNDS = 5 };

static int wait_for_children(int failed)
{
    int status;
    for (;;) {
        pid_t child = waitpid(-1, &status, 0);
        if (child == -1) {
            if (errno == EINTR)
                continue;
            return errno == ECHILD ? failed : 1;
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            failed = 1;
    }
}

int main(int argc, char *argv[])
{
    char *end;
    long rounds = argc == 2 ? strtol(argv[1], &end, 10) : -1;
    if (argc != 2 || end == argv[1] || *end != '\0' ||
        rounds < 0 || rounds > MAX_ROUNDS) {
        fprintf(stderr, "Usage: %s n (0 <= n <= %d)\n", argv[0], MAX_ROUNDS);
        return 1;
    }

    for (long round = 0; round < rounds; ++round) {
        if (fork() == -1) {
            perror("fork");
            return wait_for_children(1);
        }
        struct timespec remaining = { .tv_sec = 5 };
        while (nanosleep(&remaining, &remaining) == -1) {
            if (errno != EINTR) {
                perror("nanosleep");
                return wait_for_children(1);
            }
        }
    }

    printf("PID=%ld PPID=%ld\n", (long)getpid(), (long)getppid());
    return wait_for_children(0);
}
