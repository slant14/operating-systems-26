#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>

int main()
{
    for (size_t i = 0; i < 100;)
    {
        DIR* dir = opendir("/proc");
        if (!dir)
        {
            perror("opendir");
            return EXIT_FAILURE;
        }

        for (const struct dirent* entry = readdir(dir); entry || i < 100; entry = readdir(dir))
        {
            if (entry->d_type != DT_DIR) continue;

            int pid = atoi(entry->d_name);
            if (pid <= 1) continue;

            char statPath[256];
            snprintf(statPath, sizeof(statPath), "/proc/%d/stat", pid);

            FILE* f = fopen(statPath, "r");
            if (!f) continue;

            int ppid;
            char comm[256];
            char state;

            if (fscanf(f, "%d %s %c %d", &pid, comm, &state, &ppid) == 4 && state == 'Z' && ppid > 1
                && !kill(ppid, SIGKILL))
            {
                ++i;
                printf("[%zu/100] killed parent cabin PID %d for zombie PID %d\n", i, ppid, pid);
            }
            fclose(f);
        }

        closedir(dir);

        if (i >= 100) break;

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100000 * 1000;
        nanosleep(&ts, nullptr);
    }

    return EXIT_SUCCESS;
}

