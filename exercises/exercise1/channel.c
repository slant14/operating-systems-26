#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pts[2]; //publisher to subscriber
    const int SIZE = 1024;
    if (pipe(pts) == -1) {
        printf("error");
        return EXIT_FAILURE;
    }
    pid_t process = fork();
    if (process < 0) {
        printf("fork failed");
        return EXIT_FAILURE;
    }
    if (process == 0) {
        //child, let it be subscriber
        close(pts[1]);
        char buf[SIZE+1];
        int a;
        while ((a = (int)read(pts[0], buf, sizeof(buf) - 1)) > 0) {
            buf[a] = '\0';
            printf("The message is: %s", buf);
            fflush(stdout);
        }
        if (a < 0) {
            return EXIT_FAILURE;
        }
        buf[a] = '\0';
        printf("The message is: %s", buf);
        close(pts[0]);
    } else {
        //parent, let it be publisher
        close(pts[0]);
        char buf[SIZE];
        while (fgets(buf, SIZE, stdin) != NULL) {
            write(pts[1], buf, (int)strlen(buf));
        }
        close(pts[1]);
    }
    wait(NULL);
    return 0;
}