#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main()
{
    pid_t pid = getpid();
    FILE *pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file == NULL) return 1;
    fprintf(pid_file, "%d\n", pid);
    fclose(pid_file);

    char *password = mmap(NULL, 9, PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (password == MAP_FAILED) return 1;
    strcpy(password, "pass:");

    FILE *frandom = fopen("/dev/urandom", "r");
    if (frandom == NULL) return 1;
    for (int i = 0; i < 3; i++) {
        int ch = fgetc(frandom);
        if (ch == EOF) {
            fclose(frandom);
            return 1;
        }
        password[i+5] = 33 + ch % 94; // printable ASCII characters
    }

    password[8] = '\0';
    fclose(frandom);

    while(1) sleep(1);
}