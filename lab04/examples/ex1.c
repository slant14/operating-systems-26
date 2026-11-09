#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    int n = 4;

    // the smallest pid_t = 1 (can be <= 0)
    pid_t pid = fork();

    // always check that your sistem calls work
    if (pid < 0) {
        printf("fork failed, holy shit!\n");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // child process
        sleep(5);
        printf("hello from child! fork_pid: %d, pid: %d, value: %d, address: %p\n", pid, getpid(), n, (void *)&n);
    }

    else {
        // parent process
        kill(pid, SIGKILL);
        printf("hello from parent! fork_pid: %d, pid: %d, value: %d, address: %p\n", pid, getpid(), n, (void *)&n);
    }
}