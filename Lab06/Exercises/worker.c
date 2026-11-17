#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define TRI_BASE 1000000L

static pid_t pid;
static int process_idx;
static volatile sig_atomic_t tris = 0;

static bool is_triangular(long n) {
    if (n <= 0) return false;
    long double d = 1.0L + 8.0L * (long double)n;
    long double s = sqrtl(d);
    long long si = (long long)(s + 0.5L);
    if ((long double)si * (long double)si != d) return false;
    return ((si - 1) % 2) == 0;
}

static void signal_handler(int signum) {
    printf("Process %d (PID=<%d>): count of triangulars found so far is \e[0;31m%ld\e[0m\n",
           process_idx, pid, (long)tris);

    if (signum == SIGTSTP) {
        printf("Process %d: stopping....\n", process_idx);
        pause();
    } else if (signum == SIGCONT) {
        printf("Process %d: resuming....\n", process_idx);
    } else if (signum == SIGTERM) {
        printf("Process %d: terminating....\n", process_idx);
        exit(EXIT_SUCCESS);
    }
}

static long big_n(void) {
    long n = 0;
    while (n < TRI_BASE) n += rand();
    return n % TRI_BASE;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <process_idx>\n", argv[0]);
        return EXIT_FAILURE;
    }

    process_idx = atoi(argv[1]);
    pid = getpid();
    tris = 0;

    srand((unsigned)time(NULL) ^ (unsigned)pid);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCONT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Process %d (PID=<%d>): has been started\n", process_idx, pid);
    printf("Process %d (PID=<%d>): will find the next trianguar number from (%ld, inf)\n",
           process_idx, pid, (long)tris);

    long next_n = big_n() + 1;

    while (true) {
        if (is_triangular(next_n)) {
            printf("Process %d (PID=<%d>): I found this triangular number \e[0;31m %ld \e[0m\n",
                   process_idx, pid, next_n);
            tris++;
        }
        next_n++;
    }
}

