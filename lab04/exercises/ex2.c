#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // argv[0] is the program name
    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        fork();
        sleep(5);
    }
}

// ./a.out 3 &
// pstree -p
// this will create 8 processes in total, including the parent
// so, 7 process were created by the parent

// ./a.out 5 &
// pstree -p
// this will create 32 processes in total, including the parent
// so, 31 process were created by the parent

// in each step the number of processes doubles
// so the total num of processes is 2**n
