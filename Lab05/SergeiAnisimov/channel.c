/* Lab 05 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static int relay(int input, int output)
{
    char buffer[1024];
    for (;;) {
        ssize_t length = read(input, buffer, sizeof buffer);
        if (length == 0)
            return 0;
        if (length < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        for (ssize_t sent = 0; sent < length;) {
            ssize_t count = write(output, buffer + sent, (size_t)(length - sent));
            if (count < 0) {
                if (errno == EINTR)
                    continue;
                return -1;
            }
            sent += count;
        }
    }
}

int main(void)
{
    int channel[2];
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR || pipe(channel) == -1) {
        perror("channel setup");
        return 1;
    }

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        close(channel[0]);
        close(channel[1]);
        return 1;
    }
    if (child == 0) {
        close(channel[1]);
        int result = relay(channel[0], STDOUT_FILENO);
        if (result < 0)
            perror("subscriber");
        close(channel[0]);
        _exit(result < 0);
    }

    close(channel[0]);
    int result = relay(STDIN_FILENO, channel[1]);
    if (result < 0)
        perror("publisher");
    close(channel[1]);

    int status;
    pid_t waited;
    do {
        waited = waitpid(child, &status, 0);
    } while (waited == -1 && errno == EINTR);
    return result < 0 || waited == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0;
}
