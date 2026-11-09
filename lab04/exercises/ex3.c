#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    char command[100];

    while (1) {
        printf("> ");
        fgets(command, sizeof(command), stdin);

        int n = strlen(command);
        command[n-1] = '\0';
        printf("command: %s\n", command);

        pid_t pid = fork();
        if (pid < 0) return EXIT_FAILURE;
        if (pid == 0) {
            char path[120];
            sprintf(path, "%s/%s", "/bin", command);

            char *args[0] = {command, NULL};

            // todo: still need to add arguments to the command, if any
            // todo: add background processing 
            
            execve(path, args, NULL);
            exit(0);
        }
        wait(NULL);
    }
}