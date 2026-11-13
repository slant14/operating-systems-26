/* Lab 05 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *end = NULL;
    long id = argc == 2 ? strtol(argv[1], &end, 10) : 0;
    if (argc != 2 || end == argv[1] || *end != '\0' || id < 1 || id > INT_MAX) {
        fprintf(stderr, "Usage: %s subscriber_id\n", argv[0]);
        return 1;
    }

    const char *directory = getenv("LAB05_FIFO_DIR");
    if (directory == NULL || directory[0] == '\0')
        directory = "/tmp/ex1";

    size_t path_size = (size_t)snprintf(NULL, 0, "%s/s%ld", directory, id) + 1;
    char *path = malloc(path_size);
    if (path == NULL) {
        perror("malloc");
        return 1;
    }
    snprintf(path, path_size, "%s/s%ld", directory, id);
    struct stat info;
    if (lstat(path, &info) == -1 || !S_ISFIFO(info.st_mode)) {
        fprintf(stderr, "Expected FIFO: %s\n", path);
        free(path);
        return 1;
    }

    int input = open(path, O_RDONLY);
    if (input == -1)
        perror(path);
    free(path);
    if (input == -1)
        return 1;

    char message[1024];
    int failed = 0;
    for (;;) {
        ssize_t length = read(input, message, sizeof message);
        if (length == 0)
            break;
        if (length < 0) {
            if (errno == EINTR)
                continue;
            perror("read");
            failed = 1;
            break;
        }
        if (fwrite(message, 1, (size_t)length, stdout) != (size_t)length ||
            fflush(stdout) == EOF) {
            perror("stdout");
            failed = 1;
            break;
        }
    }
    close(input);
    return failed;
}
