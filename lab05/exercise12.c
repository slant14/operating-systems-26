#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect usage");
        return 1;
    }
    
    int n = atoi(argv[1]);

    pid_t processes[n];
    
    for (int i = 0; i < n; i++) {
        processes[i] = fork();
    
        char named_pipe[256];
        snprintf(named_pipe, 256, "/tmp/ex1/s%d",i);
        mkfifo(named_pipe, 0666);

      
        if (processes[i] < 0) {
            printf("Fork failed");
        }
        
        else if (processes[i] == 0) {
            int fd_text = open(named_pipe,O_RDONLY);
            char buff[256];
            read(fd_text,buff,256);
            printf("Here is the message:  %s\n", buff);
            exit(0);
        }
        else {
        char msg[256];
        scanf("%s",msg);
        int fd_text = open(named_pipe,O_WRONLY);
        write(fd_text,msg,strlen(msg));
        }

        for (int i = 0; i < n; i++) {
            waitpid(processes[i],NULL,0);
        }
}

}
