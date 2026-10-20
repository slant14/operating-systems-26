#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_sigalrm(int sig) {
    printf("\nchild PID: %d\n", getpid());
    exit(0);
}

int main()
{
    pid_t pid = fork();

    if (pid == 0) {
        signal(SIGALRM, handle_sigalrm);
        alarm(3);

        while (1) {
            printf("hello, world!\n");
            sleep(1);
        }
    }

    wait(NULL);
    printf("parent: child %d finished\n", pid);
}