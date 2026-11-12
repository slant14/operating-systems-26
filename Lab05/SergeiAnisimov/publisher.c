/* Lab 05 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

struct subscriber {
    pid_t pid;
    int input;
};

static volatile sig_atomic_t stopping;

static void request_stop(int signal_number)
{
    (void)signal_number;
    stopping = 1;
}

static int write_message(int fd, const char *text, ssize_t length)
{
    for (ssize_t sent = 0; sent < length;) {
        ssize_t count = write(fd, text + sent, (size_t)(length - sent));
        if (count < 0) {
            if (errno == EINTR && !stopping)
                continue;
            return -1;
        }
        sent += count;
    }
    return 0;
}

static int relay_to_subscriber(int input, const char *path)
{
    int fifo = open(path, O_WRONLY);
    if (fifo == -1)
        return -1;

    char message[1024];
    int result = 0;
    for (;;) {
        ssize_t length = read(input, message, sizeof message);
        if (length == 0)
            break;
        if (length < 0 && errno == EINTR)
            continue;
        if (length < 0 || write_message(fifo, message, length) < 0) {
            result = -1;
            break;
        }
    }
    close(fifo);
    return result;
}

int main(int argc, char *argv[])
{
    char *end = NULL;
    long total = argc == 2 ? strtol(argv[1], &end, 10) : 0;
    if (argc != 2 || end == argv[1] || *end != '\0' || total < 1 || total > INT_MAX) {
        fprintf(stderr, "Usage: %s subscriber_count\n", argv[0]);
        return 1;
    }

    const char *directory = getenv("LAB05_FIFO_DIR");
    if (directory == NULL || directory[0] == '\0')
        directory = "/tmp/ex1";

    struct sigaction action = { .sa_handler = request_stop };
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) == -1 || sigaction(SIGTERM, &action, NULL) == -1 ||
        signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal setup");
        return 1;
    }

    struct subscriber *subscribers = calloc((size_t)total, sizeof *subscribers);
    if (subscribers == NULL) {
        perror("calloc");
        return 1;
    }
    if (mkdir(directory, 0700) == -1) {
        perror(directory);
        free(subscribers);
        return 1;
    }

    size_t path_size = (size_t)snprintf(NULL, 0, "%s/s%ld", directory, total) + 1;
    char *path = malloc(path_size);
    if (path == NULL) {
        perror("malloc");
        rmdir(directory);
        free(subscribers);
        return 1;
    }

    long created = 0;
    long fifo_count = 0;
    int failed = 0;
    for (; created < total && !stopping; ++created) {
        snprintf(path, path_size, "%s/s%ld", directory, created + 1);
        if (mkfifo(path, 0600) == -1) {
            perror("mkfifo");
            failed = 1;
            break;
        }
        ++fifo_count;

        int channel[2];
        if (pipe(channel) == -1) {
            perror("pipe");
            failed = 1;
            break;
        }
        pid_t child = fork();
        if (child == -1) {
            perror("fork");
            close(channel[0]);
            close(channel[1]);
            failed = 1;
            break;
        }
        if (child == 0) {
            signal(SIGINT, SIG_DFL);
            signal(SIGTERM, SIG_DFL);
            close(channel[1]);
            for (long i = 0; i < created; ++i)
                close(subscribers[i].input);
            int result = relay_to_subscriber(channel[0], path);
            if (result < 0)
                perror("publish to subscriber");
            close(channel[0]);
            _exit(result < 0);
        }
        close(channel[0]);
        subscribers[created] = (struct subscriber){child, channel[1]};
    }

    char message[1024];
    while (!failed && !stopping) {
        ssize_t length = read(STDIN_FILENO, message, sizeof message);
        if (length == 0)
            break;
        if (length < 0) {
            if (errno == EINTR)
                continue;
            perror("read");
            failed = 1;
            break;
        }
        for (long i = 0; i < created; ++i) {
            if (write_message(subscribers[i].input, message, length) < 0) {
                if (!stopping)
                    perror("send");
                failed = !stopping;
                break;
            }
        }
    }

    for (long i = 0; i < created; ++i) {
        close(subscribers[i].input);
        if (failed || stopping)
            kill(subscribers[i].pid, SIGTERM);
    }
    for (long i = 0; i < created; ++i) {
        pid_t child;
        int status;
        do {
            child = waitpid(subscribers[i].pid, &status, 0);
            if (child < 0 && errno == EINTR && stopping)
                kill(subscribers[i].pid, SIGTERM);
        } while (child < 0 && errno == EINTR);
        if (!stopping && (child < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0))
            failed = 1;
    }
    for (long i = 1; i <= fifo_count; ++i) {
        snprintf(path, path_size, "%s/s%ld", directory, i);
        unlink(path);
    }
    rmdir(directory);
    free(path);
    free(subscribers);
    return failed;
}
