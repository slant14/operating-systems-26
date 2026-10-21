#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

static volatile sig_atomic_t want_read = 0;
static volatile sig_atomic_t want_exit = 0;

static void on_sigusr1(int signo) {
    (void)signo;
    want_read = 1;
}

static void on_sigusr2(int signo) {
    (void)signo;
    want_exit = 1;
}

int main(void) {
    FILE *textFile = fopen("text.txt", "r");
    if (!textFile) {
        perror("fopen text.txt");
        return 1;
    }

    if (mkdir("/tmp/run", 0777) == -1 && errno != EEXIST) {
        perror("mkdir /tmp/run");
        fclose(textFile);
        return 1;
    }

    FILE *fp = fopen("/tmp/run/agent.pid", "w");
    if (!fp) {
        perror("fopen agent.pid");
        fclose(textFile);
        return 1;
    }
    fprintf(fp, "%d\n", getpid());
    fclose(fp);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);
    sa.sa_handler = on_sigusr2;
    sigaction(SIGUSR2, &sa, NULL);

    char content[1024];

    if (fgets(content, sizeof(content), textFile)) {
        fputs(content, stdout);
    }

    while (!want_exit) {
        pause(); // sleep until a signal arrives

        if (want_read) {
            want_read = 0;
            if (fgets(content, sizeof(content), textFile)) {
                fputs(content, stdout);
            } else {
                // EOF reached
                puts("[EOF]");
            }
        }
    }

    puts("Process terminating...");
    fclose(textFile);
    return 0;
}

