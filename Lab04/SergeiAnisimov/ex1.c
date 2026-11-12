/* Lab 04 Sergei Anisimov CSE-04 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void print_process(const char *role, clock_t start)
{
    clock_t finish = clock();
    double milliseconds = 1000.0 * (finish - start) / CLOCKS_PER_SEC;
    printf("%s PID=%ld PPID=%ld CPU_ms=%.3f\n",
           role, (long)getpid(), (long)getppid(), milliseconds);
}

int main(void)
{
    pid_t children[2];
    size_t created = 0;
    clock_t parent_start = 0;
    int failed = 0;

    for (; created < 2; ++created) {
        pid_t child = fork();
        clock_t start = clock();
        if (child == -1) {
            perror("fork");
            failed = 1;
            break;
        }
        if (child == 0) {
            print_process("Child", start);
            _exit(fflush(stdout) == EOF ? EXIT_FAILURE : EXIT_SUCCESS);
        }
        if (created == 0)
            parent_start = start;
        children[created] = child;
    }

    for (size_t i = 0; i < created; ++i) {
        int status;
        pid_t result;
        do {
            result = waitpid(children[i], &status, 0);
        } while (result == -1 && errno == EINTR);
        if (result == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
            failed = 1;
    }

    if (created > 0)
        print_process("Parent", parent_start);
    return failed;
}
