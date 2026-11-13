/* Lab 06 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef TRI_BASE
#define TRI_BASE 1000000
#endif

static volatile sig_atomic_t stopping;

static void on_signal(int signal_number)
{
    if (signal_number == SIGTERM)
        stopping = 1;
    else if (signal_number == SIGTSTP)
        kill(getpid(), SIGSTOP);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 2;
    struct sigaction action = {.sa_handler = on_signal};
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGTSTP, &action, NULL) == -1 ||
        sigaction(SIGCONT, &action, NULL) == -1 ||
        sigaction(SIGTERM, &action, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
    sigset_t termination;
    sigemptyset(&termination);
    sigaddset(&termination, SIGTERM);
    if (sigprocmask(SIG_UNBLOCK, &termination, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }
    srand((unsigned int)getpid());
    unsigned long long start = 1 + rand() % TRI_BASE;
    unsigned long long triangular = 1, step = 1, count = 0;
    while (triangular <= start)
        triangular += ++step;
    printf("Worker %s started after %llu (TRI_BASE=%d)\n", argv[1], start, TRI_BASE);
    fflush(stdout);
    raise(SIGSTOP);
    for (unsigned long long candidate = start + 1; !stopping; ++candidate) {
        if (candidate == triangular) {
            ++count;
            triangular += ++step;
        }
    }
    printf("Worker %s found %llu triangular numbers\n", argv[1], count);
    return 0;
}
