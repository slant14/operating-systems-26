#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    FILE * file = fopen("/tmp/ex1.pid", "w");
    if (file == NULL) {
        return EXIT_FAILURE;
    }
    fprintf(file, "%d\n", getpid());
    fclose(file);

    char *password = mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (password == MAP_FAILED) {
        printf("mmap failed.\n");
        return EXIT_FAILURE;
    }
    strcpy(password, "pass:");

    // 8 random printable characters after the prefix
    FILE *rnd = fopen("/dev/urandom", "r");
    if (rnd == NULL) {
        return EXIT_FAILURE;
    }
    int len = 5;
    while (len < 13) {
        int c = fgetc(rnd);
        if (isprint(c)) {
            password[len] = c;
            len++;
        }
    }
    password[len] = '\0';
    fclose(rnd);

    while (1) {
        sleep(1);
    }
    return 0;
}