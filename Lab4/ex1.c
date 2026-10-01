#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

int main() {
    pid_t process1 = fork();
    if (process1 < 0) {
        printf("fork failed.");
        return EXIT_FAILURE;
    }
    if (process1 != 0) {
        pid_t process2 = fork();
    }
    clock_t start = clock();
    printf("ID of the process is %d, its parent's ID is %d\n", getpid(), getppid());
    clock_t end = clock();
    printf("Execution time of process with pid %d is %f\n", getpid(), (float)(end-start)/CLOCKS_PER_SEC);
    return 0;
}