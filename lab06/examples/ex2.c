#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("\ninterrupted!\n");
    signal(SIGINT, SIG_DFL);
}

int main()
{
    signal(SIGINT, handle_sigint);
    printf("pid: %d\n", getpid());

    while (1) {
        printf("hello, world!\n");
        sleep(1);
    }
}