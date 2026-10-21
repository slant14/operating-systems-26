#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>
int main(int argc, char* argv[]) {
    if (argc != 2 ) {
        printf("Incorrect usage");
        return 1;
    }

    char* msg = argv[1];
    int pts[2];

    if (pipe(pts) == -1) {
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    }
    else if (pid == 0) { 
        write(pts[1],msg,strlen(msg));
    }
    else {

        char buff[1024];
        int n = (int)read(pts[0],buff,sizeof(buff) - 1);

        if (n<0) {
            return 1;
        }

        buff[n] = '\0';
        printf("Here is the message %d", buff);
    }

    wait(NULL);

}
