#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

pid_t agent_pid;

void sigint_handler(int sig) {
    kill(agent_pid, SIGTERM);
    exit(EXIT_SUCCESS);
}

int main() {
    FILE * file = fopen("/var/run/agent.pid", "r");
    int pid;
    if (file == NULL || fscanf(file, "%d", &pid) != 1) {
        printf("Error: No agent found.\n");
        return EXIT_FAILURE;
    }
    fclose(file);
    agent_pid = pid;
    // signal 0 only checks that the process exists
    if (kill(agent_pid, 0) == -1) {
        printf("Error: No agent found.\n");
        return EXIT_FAILURE;
    }
    printf("Agent found.\n");
    signal(SIGINT, sigint_handler);

    char cmd[16];
    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent\n");
        if (scanf("%15s", cmd) != 1) {
            return EXIT_FAILURE;
        }
        if (strcmp(cmd, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(cmd, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            return 0;
        } else if (strcmp(cmd, "stop") == 0) {
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(cmd, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        }
    }
    return 0;
}