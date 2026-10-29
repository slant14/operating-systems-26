#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void print_content() {
    FILE * file = fopen("text.txt", "r");
    if (file == NULL) {
        printf("error with opening text.txt\n");
        return;
    }
    char buf[256];
    while (fgets(buf, sizeof(buf), file) != NULL) {
        printf("%s", buf);
    }
    fclose(file);
}

void read_handler(int sig) {
    print_content();
}

void exit_handler(int sig) {
    printf("Process terminating...\n");
    remove("/var/run/agent.pid"); // so the controller doesn't find a dead agent
    exit(EXIT_SUCCESS);
}

int main() {
    FILE * file = fopen("/var/run/agent.pid", "w");
    if (file == NULL) {
        return EXIT_FAILURE;
    }
    fprintf(file, "%d\n", getpid());
    fclose(file);

    signal(SIGUSR1, read_handler);
    signal(SIGUSR2, exit_handler);

    print_content();
    while (1) {
        pause(); // sleep until a signal comes
    }
    return 0;
}