#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char* argv[]) {
    //input from command line, not from stdin
    if (argc < 2) {
        return 1;
    }
    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        fork();
        sleep(5);
    }

    return 0;
}
//gcc exercise2.c -o e42
//./e42 5 & - the execution of the program is sent to the background with n = 5
//For n=3 there will be 8 processes created (2^3 = 8), for n=5 there will be 32 processes created (2^5 = 32). By calling the command pstree several times, we can gradually notice the tree of processes, for n = 5 I observed 2 processes, then 4, 8, 16, and finally 32 processes.
