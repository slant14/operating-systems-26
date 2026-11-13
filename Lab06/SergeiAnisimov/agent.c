/* Lab 06 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static int print_message(void)
{
    FILE *message = fopen("text.txt", "r");
    if (!message) {
        perror("text.txt");
        return 1;
    }
    int ch;
    while ((ch = fgetc(message)) != EOF)
        putchar(ch);
    int result = ferror(message) || fflush(stdout) == EOF;
    fclose(message);
    return result;
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [pid-file]\n", argv[0]);
        return 2;
    }
    const char *path = argc == 2 ? argv[1] : "/var/run/agent.pid";
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    sigaddset(&signals, SIGUSR2);
    sigaddset(&signals, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }
    FILE *pid_file = fopen(path, "wx");
    if (!pid_file) {
        perror(path);
        return 1;
    }
    int result = fprintf(pid_file, "%ld\n", (long)getpid()) < 0;
    result |= fclose(pid_file) == EOF;
    if (!result)
        result = print_message();

    while (!result) {
        int signal_number = sigwaitinfo(&signals, NULL);
        if (signal_number == SIGUSR1)
            result = print_message();
        else if (signal_number == SIGUSR2 || signal_number == SIGTERM) {
            puts("Process terminating...");
            break;
        } else if (errno != EINTR) {
            perror("sigwaitinfo");
            result = 1;
        }
    }

    if (unlink(path) == -1) {
        perror(path);
        result = 1;
    }
    return result;
}
