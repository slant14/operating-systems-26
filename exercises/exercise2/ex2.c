#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

struct Thread {
    unsigned int id;
    int i;
    char message[256];
};

int main() {
    int n = 10;
    struct Thread threads[n];
    pid_t proc;
    for (int i = 0; i < n; i++) {
        proc = fork();
        if (proc < 0) {
            printf("fork failed\n");
            return EXIT_FAILURE;
        }
        if (proc == 0) {
            //child
            threads[i].id = getpid();
            threads[i].i = i;
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Hello from thread %d\n", i);
            for (int  j = 0; j < 256; j++) {
                threads[i].message[j] = buffer[j];
            }
            printf("ID is %d, %s\n", threads[i].id, threads[i].message);
            exit(0);
        } else {
            printf("Thread %d is created\n", i);
            fflush(stdout);
            wait(NULL);
        }
    }
    return 0;
}