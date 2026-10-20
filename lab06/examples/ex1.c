#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("\ninterrupted!\n");
}

int main()
{
    signal(SIGINT, handle_sigint);
    printf("%d", getpid());
    sleep(1);
    while (1) printf("hello, world!\n");
}