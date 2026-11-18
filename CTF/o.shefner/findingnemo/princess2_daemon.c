#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(void) {
    pid_t p = fork();
    if (p < 0) return 1;
    if (p > 0) { printf("Started blade PID %ld\n", (long)p); return 0; }
    if (setsid() < 0) _exit(2);
    int in = open("/dev/null", O_RDONLY);
    int out = open("/home/ctf/princess2.log", O_WRONLY|O_CREAT|O_APPEND, 0600);
    if (in >= 0) { dup2(in, 0); close(in); }
    if (out >= 0) { dup2(out, 1); dup2(out, 2); close(out); }
    execl("/home/ctf/blade", "blade", (char *)0);
    perror("execl blade");
    _exit(3);
}
