#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

// pipes
// half-duplex method

int main(int argc, char* argv[])
{
    // GOAL ./main "a message"
    if (argc != 2) {
        printf("incorrect usage\n");
        return 1;
    }

    char *msg = argv[1];
    int ptc[2];
    int ctp[2];

    if (pipe(ptc) == -1 || pipe(ctp) == -1) {
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {

        // 1 - write

        close(ptc[1]); // close write part 
        close(ctp[0]); // close read part

        char buf[256];
        int n = (int)read(ptc[0], buf, sizeof(buf) - 1);
        if (n < 0) return 1;
        buf[n] = '\0';

        printf("hello from child: the message is %s\n", buf);

        int size = (int)strlen(buf);
        write(ctp[1], &size, sizeof(size));

        close(ptc[0]);
        close(ctp[1]);
    }
    else {
        close(ptc[0]);
        close(ctp[1]);
        write(ptc[1], msg, (int)strlen(msg));
        
        int len = -1;
        read(ctp[0], &len, sizeof(len));
        printf("hello from parent: the size of message is %d\n", len);

        close(ptc[1]);
        close(ctp[0]);
    }
    wait(NULL);
}