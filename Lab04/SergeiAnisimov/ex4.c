/* Lab 04 Sergei Anisimov CSE-04 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

enum { MAX_ARGS = 64 };

static void run_command(char *args[], bool background)
{
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return;
    }
    if (child == 0) {
        if (background) {
            int input = open("/dev/null", O_RDONLY);
            if (input == -1 || dup2(input, STDIN_FILENO) == -1) {
                perror("/dev/null");
                _exit(1);
            }
            if (input != STDIN_FILENO)
                close(input);
        }
        execvp(args[0], args);
        int error = errno;
        perror(args[0]);
        _exit(error == ENOENT ? 127 : 126);
    }

    if (background) {
        printf("Background PID: %ld\n", (long)child);
        return;
    }

    pid_t result;
    do {
        result = waitpid(child, NULL, 0);
    } while (result == -1 && errno == EINTR);
    if (result == -1)
        perror("waitpid");
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    bool interactive = isatty(STDIN_FILENO);

    for (;;) {
        while (waitpid(-1, NULL, WNOHANG) > 0)
            continue;
        if (interactive) {
            fputs("lab04> ", stdout);
            fflush(stdout);
        }
        if (getline(&line, &capacity, stdin) < 0)
            break;

        char *cursor;
        char *word = strtok_r(line, " \t\r\n", &cursor);
        if (word == NULL)
            continue;
        if (strcmp(word, "exit") == 0)
            break;

        bool background = strcmp(word, "bg") == 0;
        if (background)
            word = strtok_r(NULL, " \t\r\n", &cursor);
        if (word == NULL) {
            fputs("Usage: bg command [arguments]\n", stderr);
            continue;
        }

        char *args[MAX_ARGS + 1];
        size_t count = 0;
        while (word != NULL && count < MAX_ARGS) {
            args[count++] = word;
            word = strtok_r(NULL, " \t\r\n", &cursor);
        }
        if (word != NULL) {
            fputs("Too many arguments.\n", stderr);
            continue;
        }
        args[count] = NULL;
        run_command(args, background);
    }

    free(line);
    return ferror(stdin) ? 1 : 0;
}
