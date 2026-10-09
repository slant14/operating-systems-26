#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int SIZE = 1024;
    if (argc != 2) {
        printf("incorrect usage\n");
        return 1;
    }
    int id = atoi(argv[1]);
    char path[64];
    snprintf(path, sizeof(path), "/tmp/ex1/s%d", id); 
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("error");
        return EXIT_FAILURE;
    }
    char buf[SIZE+1];
    int n;
    while (n = (int)read(fd, buf, sizeof(buf) - 1) > 0) {
        buf[n] = '\0';
        printf("The message is: %s. It was subscriber %d", buf, id);
        fflush(stdout);
    }
    if (n < 0) {
        return EXIT_FAILURE;
    }
    close(fd);
    return 0;
}