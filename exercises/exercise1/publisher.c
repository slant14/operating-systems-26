#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("incorrect usage\n");
        return EXIT_FAILURE;
    }
    int n = atoi(argv[1]);
    if (n < 1 || n > 3) {
        printf("incorrect number\n");
        return EXIT_FAILURE;
    }
    signal(SIGPIPE, SIG_IGN); //if a subscriber disappears
    if (mkdir("/tmp/ex1", 0777) == -1 && errno != EEXIST) {
        printf("error");
        return EXIT_FAILURE;
    }
    pid_t proc;
    char pipes[n][64];
    int pts[n][2];
    const int SIZE = 1024;

    for (int i = 0; i < n; i++) {
        snprintf(pipes[i], sizeof(pipes[i]), "/tmp/ex1/s%d", i + 1);
        int res = mkfifo(pipes[i], 0666);
        if (res < 0 && errno != EEXIST) {
            printf("error");
            return EXIT_FAILURE;
        }
        if (pipe(pts[i]) == -1) {
            printf("error");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < n; i++) {
        proc = fork();
        if (proc < 0) {
            printf("fork failed");
            return EXIT_FAILURE;
        }
        if (proc == 0) {
            //child
            for (int j = 0; j < n; j++) {
                close(pts[j][1]);
                if (j != i) {
                    close(pts[j][0]);
                }
            }
            int fd = open(pipes[i], O_WRONLY);
            if (fd < 0) {
                printf("error");
                return EXIT_FAILURE;
            }
            char buf[SIZE];
            int r;
            while ((r = (int)read(pts[i][0], buf, SIZE)) > 0) {
                if (write(fd, buf, r) < 0) {
                    break;
                }
            }
            close(fd);
            close(pts[i][0]);
            exit(0);
        }
    }
    //parent cont.
    for (int i = 0; i < n; i++) {
        close(pts[i][0]);
    }
    char buf[SIZE];
    while (fgets(buf, SIZE, stdin) != NULL) {
        for (int i = 0; i < n; i++) {
            write(pts[i][1], buf, strlen(buf));
        }
    }
    for (int i = 0; i < n; i++) {
        close(pts[i][1]);
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    return 0;
}