// zombie has the resources but does not do anything
// Z+ - zombie in foreground
// parent sleeping

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) return EXIT_FAILURE;
    if (pid == 0) {
        printf("%d, child\n", getpid());
    }
    else {
        sleep(15);
        printf("parent\n");
    }
}

// orphan 
// child sleeping, parent finishes, so someone should take them (root, for example)