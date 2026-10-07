#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
pid_t processes[2];
int main() {
    clock_t t0 = clock();

    for (int i = 0; i < 2; i++) {
    processes[i] = fork();
    if (processes[i] < 0) {
        printf("Fork failed");
    }
    else if (processes[i] == 0) {
        clock_t t1 = clock();
        printf("I am a child %d\n", getpid());
        printf("%f\n",(((float)clock()-t1)/CLOCKS_PER_SEC)*1000);
        exit(0);
    }
    else {
        printf("I am a parent %d\n", getpid());
        clock_t t2 = clock() - t0;
        printf("%f\n",(((float)t2)/CLOCKS_PER_SEC)*1000);
    }
    }
    for (int i=0;i<2;i++) {
        waitpid(processes[i],NULL,0);
    }
}
