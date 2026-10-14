#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int pts[2];
    int stp[2];

    if (pipe(pts) == -1 || pipe(stp) == -1) {
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        // subscriber
        close(pts[1]);
        close(stp[0]);
    }
    else {
        // publisher
        close(pts[0]);
        close(stp[1]);

        char message[1024];
        int n = (int)read(pts[1], message, sizeof(message) - 1);
        if (n < 0) return 1;
        message[n] = '\0';

        close(pts[1]);
        close(stp[0]);
    }
}