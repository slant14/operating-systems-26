#include <stdio.h>  // io
#include <unistd.h> // fork()
#include <time.h>   // clock()
#include <stdlib.h> // exit()
#include <sys/wait.h> // wait()

int main()
{
    pid_t pid1 = fork();
    clock_t start1 = clock();
    if (pid1 < 0) return EXIT_FAILURE;
    if (pid1 == 0) {
        printf("child 1 pid: %d, parent pid: %d\n", getpid(), getppid());
        clock_t end = clock();
        double time = (double)(end - start1) * 1000.0 / CLOCKS_PER_SEC;
        printf("child 1 time: %f ms\n", time);
        exit(0);
    }

    pid_t pid2 = fork();
    clock_t start2 = clock();
    if (pid2 < 0) return EXIT_FAILURE;
    if (pid2 == 0) {
        printf("child 2 pid: %d, parent pid: %d\n", getpid(), getppid());
        clock_t end = clock();
        double time = (double)(end - start2) * 1000.0 / CLOCKS_PER_SEC;
        printf("child 2 time: %f ms\n", time);
        exit(0);
    }

    for (int i = 0; i < 2; i++) wait(NULL);

    printf("parent pid: %d, grandpa pid: %d\n", getpid(), getppid());
    clock_t end = clock();
    double time = (double)(end - start1) * 1000.0 / CLOCKS_PER_SEC;
    printf("parent time: %f ms\n", time);
    exit(0);
}

// child 1 pid: 16169, parent pid: 16168
// child 1 time: 0.078000 ms
// child 2 pid: 16170, parent pid: 16168
// child 2 time: 0.117000 ms
// parent pid: 16168, grandpa pid: 14829
// parent time: 0.225000 ms