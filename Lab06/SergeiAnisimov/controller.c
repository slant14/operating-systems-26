/* Lab 06 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t interrupted;

static void on_interrupt(int signal_number)
{
    (void)signal_number;
    interrupted = 1;
}

static int terminate_agent(pid_t pid, int signal_number)
{
    if (kill(pid, SIGCONT) == -1 || kill(pid, signal_number) == -1) {
        perror("terminate agent");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [pid-file]\n", argv[0]);
        return 2;
    }
    FILE *file = fopen(argc == 2 ? argv[1] : "/var/run/agent.pid", "r");
    long pid = -1;
    if (file) {
        if (fscanf(file, "%ld", &pid) != 1)
            pid = -1;
        fclose(file);
    }
    if (pid < 2 || pid > INT_MAX || kill((pid_t)pid, 0) == -1) {
        puts("Error: No agent found.");
        return 1;
    }
    struct sigaction action = {.sa_handler = on_interrupt};
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    puts("Agent found.");
    char command[32];
    while (!interrupted) {
        puts("Choose a command {read, exit, stop, continue} to send to the agent");
        fflush(stdout);
        if (!fgets(command, sizeof command, stdin))
            break;
        command[strcspn(command, "\r\n")] = '\0';
        int signal_number;
        if (strcmp(command, "read") == 0)
            signal_number = SIGUSR1;
        else if (strcmp(command, "exit") == 0)
            return terminate_agent((pid_t)pid, SIGUSR2);
        else if (strcmp(command, "stop") == 0)
            signal_number = SIGSTOP;
        else if (strcmp(command, "continue") == 0)
            signal_number = SIGCONT;
        else {
            fputs("Unknown command.\n", stderr);
            continue;
        }
        if (kill((pid_t)pid, signal_number) == -1) {
            perror("signal to agent");
            return 1;
        }
    }
    return interrupted ? terminate_agent((pid_t)pid, SIGTERM) : ferror(stdin) != 0;
}
