// orphan 
// child sleeping, parent finishes, so someone should take them (root, for example)

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) return EXIT_FAILURE;
    if (pid == 0) {
        sleep(8);
        printf("%d, child\n", getpid());
    }
    else {
        printf("parent\n");
    }
}