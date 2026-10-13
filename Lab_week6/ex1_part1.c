#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_MESSAGE 1024

int main() {
    int fd[2];
    pid_t pid;
    char message[MAX_MESSAGE];

    if (pipe(fd) == -1) {
        printf("Pipe creation failed\n");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    // Child
    if (pid == 0) {
        close(fd[1]);

        char buffer[MAX_MESSAGE];
        ssize_t n;

        while ((n = read(fd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);
        }

        close(fd[0]);
        return 0;

    } else {
        // Parent
        close(fd[0]);

        while (fgets(message, sizeof(message), stdin) != NULL) {
            size_t len = strlen(message);

            if (len > MAX_MESSAGE) {
                printf("Message too long!\n");
                return 1;
            }

            write(fd[1], message, len);
        }

        close(fd[1]);
    }

    return 0;
}

